//
//  RoundSchedule.cpp
//  GA
//
//  Created by Tom den Ottelander on 22/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "RoundSchedule.hpp"

using namespace std;
using nlohmann::json;
using Utility::millis;

extern bool printPopulationAfterRound;
extern bool printPopulationOnOptimum;
extern bool storeConvergence;

nlohmann::json convergence;

RoundSchedule::RoundSchedule (int maxRounds, int maxPopSizeLevel, int maxSeconds, int maxEvaluations, int interleavedRoundInterval) :
    maxRounds(maxRounds),
    maxPopSizeLevel(maxPopSizeLevel),
    maxSeconds(maxSeconds),
    maxEvaluations(maxEvaluations),
    interval(interleavedRoundInterval)
{}

void RoundSchedule::initialize(GA *g, int problemSize, bool IMS, int nonIMSpopsize) {
    int beginPopSize = 2;
    if(!IMS){
        maxPopSizeLevel = 1;
        beginPopSize = nonIMSpopsize;
    }
    gaList.reserve(maxPopSizeLevel);

    output["fitness"] = g->fitFunc_ptr->id();
    output["successfulGAPopulation"] = -1;
    output["successfulGARoundCount"] = -1;
    output["success"] = false;
    output["stoppingCondition"] = "-1";
    
    g->fitFunc_ptr->setLength(problemSize);
    
    //TODO: Refactor as armadillo uvec
    whichShouldRun.reserve(maxPopSizeLevel);
    for(int i = 0; i < maxPopSizeLevel; i++){
        whichShouldRun.push_back(0);
        int popSize = pow(beginPopSize, i + 1);
        GA* newGA = g->clone();
        newGA->setPopulationSize(popSize);
        gaList.push_back(newGA);
    }
    whichShouldRun[0] = 1;
    
    convergence.clear();
    convergence["absolute"] = {};
    convergence["unique"] = {};
}

json RoundSchedule::run() {
    int round = 0;
    int lowestActiveGAIdx = 0;
    bool optimumFound = false;
    bool done = false;
    long start = millis();
    while (!done) {

        //Stopping conditions
        if (maxRounds != -1 && round >= maxRounds) {
            output["stoppingCondition"] = "maxRoundsExceeded";
            break;
        } else if (maxSeconds != -1 && millis() - start > maxSeconds * 1000) {
            output["stoppingCondition"] = "maxTimeExceeded";
            break;
        } else if (maxEvaluations != -1 && gaList[0]->fitFunc_ptr->totalEvaluations > maxEvaluations){
            output["stoppingCondition"] = "maxEvaluationsExceeded";
            break;
        }

        //TODO: Loop only through active GA's, not until maxPopSizeLevel
        for (int i = lowestActiveGAIdx; i < maxPopSizeLevel; i++) {

            //First check if GA is not terminated
            if (!gaList[i]->terminated) {

                //If this is the first non-terminated GA, always do the round
                if(i == lowestActiveGAIdx){
                    whichShouldRun[i] = 1;
                }

                //Check if this GA should be run
                if(whichShouldRun[i] == 1){
                    //If it is not the first non-terminated GA, don't run it in the next cycle
                    if(i != lowestActiveGAIdx){
                        whichShouldRun[i] = 0;
                    }

                    // Create a pointer to the current GA
                    GA* ga = gaList[i];

                    // Initialize the GA if that has not been done yet
                    if(!ga->initialized){
                        ga->initialize();
                        // Define the first ever individual as bestIndividualOverall
                        if(i == 0){
                            ga->fitFunc_ptr->bestIndividual = ga->population[0].copy();
                        }
                        ga->evaluateAll();
                    }
                    
                    // Do the round on this GA
                    ga->round();
                    if(printPopulationAfterRound) ga->print();
                    
                    if (maxEvaluations != -1 && ga->fitFunc_ptr->totalEvaluations > maxEvaluations){
                        break;
                    }
                    
                    // If the current GA has found the optimum, break out of the loop
                    if (ga->isOptimal()){
                        if(printPopulationOnOptimum) ga->print();
                        optimumFound = true;
                        output["successfulGAPopulation"] = ga->populationSize;
                        output["successfulGARoundCount"] = ga->roundsCount;
                        break;

                        // Else if the GA is converged, terminate this GA and all before.
                    } else if (ga->isConverged()){
                        // GA.termination is already set to true inside isConverged()-method
                        // Set all previous GA's to terminated
                        terminateGAs(i);
                        lowestActiveGAIdx = i + 1;
                        // Break out, because the GA with highest popSize has converged
                        if(lowestActiveGAIdx == maxPopSizeLevel){
                            done = true;
                            break;
                        }
                        continue;

                        // Else if it has a higher fitness than the previous GA, terminate all before this one
                    } else if (i != 0 && !gaList[i-1]->terminated && ga->getAvgFitness() > gaList[i-1]->getAvgFitness()){
                        terminateGAs(i - 1);
                        lowestActiveGAIdx = i;
                        continue;
                    }

                    // If this GA has run 4 times, make sure the next GA also does a run
                    if ((gaList[i]->roundsCount) % interval == 0){
                        whichShouldRun[i+1] = 1;
                    }
                }
            }
        }

        if(optimumFound){
            output["success"] = true;
            output["stoppingCondition"] = "optimumReached";
            break;
        }

        //Increment counter
        round++;
    }
    
    long stop = millis();
    output["timeTaken"] = stop - start;
    output["evaluations"] = gaList[0]->fitFunc_ptr->totalEvaluations;
    output["uniqueEvaluations"] = gaList[0]->fitFunc_ptr->totalUniqueEvaluations;
    output["transformedUniqueEvaluations"] = gaList[0]->fitFunc_ptr->totalTransformedUniqueEvaluations;
    if (storeConvergence)
        output["convergence"] = convergence;
    
    return output;
}

// Terminate ga's in gaList up to and including index n
void RoundSchedule::terminateGAs(int n){
    for(int i = 0; i < (n + 1); i++){
        (*gaList[i]).terminated = true;
    }
}

int RoundSchedule::getAmountOfEvaluations(){
    int evaluations = 0;
    for (GA *ga : gaList) {
        evaluations += ga->getTotalAmountOfEvaluations();
    }
    return evaluations;
}
