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

RoundSchedule::RoundSchedule (int maxRounds, int maxPopSizeLevel, int maxSeconds, int maxEvaluations, int maxUniqueEvaluations, int interleavedRoundInterval) :
    maxRounds(maxRounds),
    maxPopSizeLevel(maxPopSizeLevel),
    maxSeconds(maxSeconds),
    maxEvaluations(maxEvaluations),
    maxUniqueEvaluations(maxUniqueEvaluations),
    interval(interleavedRoundInterval)
{}

void RoundSchedule::initialize(GA *g, int problemSize, bool IMS, int nonIMSpopsize) {
    int beginPopSize = 4;
    if(!IMS || g->isLocalSearchAlgorithm){
        maxPopSizeLevel = 1;
        beginPopSize = nonIMSpopsize;
    }
    gaList.reserve(maxPopSizeLevel);

    output["fitness"] = g->fitFunc_ptr->id();
    output["successfulGAPopulation"] = -1;
    output["successfulGARoundCount"] = -1;
    output["popsizereached"] = -1;
    output["success"] = false;
    output["stoppingCondition"] = "-1";
    
    g->fitFunc_ptr->setLength(problemSize);
    g->fitFunc_ptr->maxEvaluations = maxEvaluations;
    g->fitFunc_ptr->maxUniqueEvaluations = maxUniqueEvaluations;
    
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
    int highestActiveGAIdx = 0;
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
        } else if (maxEvaluationsExceeded()){
            output["stoppingCondition"] = "maxEvaluationsExceeded";
            break;
        } else if (maxUniqueEvaluationsExceeded()){
            output["stoppingCondition"] = "maxUniqueEvaluationsExceeded";
            break;
        }

        //TODO: Loop only through active GA's, not until maxPopSizeLevel
        for (int i = lowestActiveGAIdx; i <= highestActiveGAIdx; i++) {

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
                    ga = gaList[i];

                    // Initialize the GA if that has not been done yet
                    if(!ga->initialized){
                        ga->initialize();
                        output["popsizereached"] = ga->populationSize;
                        
                        // Define the first ever individual as bestIndividualOverall
                        if(i == 0){
                            ga->fitFunc_ptr->bestIndividual = ga->population[0].copy();
                        }
                        ga->evaluateAll();
                    }
                    
                    // Do the round on this GA
//                    ga->print();
//                    cout << "Performing round with popsize " << ga->populationSize << endl;
                    ga->round();
                    if(printPopulationAfterRound) ga->print();
                    
                    if (maxEvaluationsExceeded() || maxUniqueEvaluationsExceeded()){
                        break;
                    }
                    
                    // If the current GA has found the optimum, break out of the loop
                    if (ga->isOptimal()){
//                        cout << "ga " << ga->populationSize << " is optimal" << endl;
                        if(printPopulationOnOptimum) ga->print();
                        optimumFound = true;
                        output["successfulGAPopulation"] = ga->populationSize;
                        output["successfulGARoundCount"] = ga->roundsCount;
                        break;

                    // Else if the GA is converged, terminate this GA and all before.
                    } else if (ga->isConverged()){
                        // GA.termination is already set to true inside isConverged()-method
                        // Set all previous GA's to terminated
//                        cout << "Terminate ga " << ga->populationSize << " due to convergence" << endl;
                        terminateGAs(i);
                        lowestActiveGAIdx = i + 1;
                        // Break out, because the GA with highest popSize has converged
                        if(lowestActiveGAIdx == maxPopSizeLevel){
                            done = true;
                            break;
                        }
                        continue;

                    // Else if the previous GA has not terminated
                    } else if (i != 0 && !gaList[i-1]->terminated){
                        
                        // If this is a SO problem, check if this GA has a higher fitness than the previous GA. If so, terminate previous GA and all before.
                        if (ga->fitFunc_ptr->numObjectives == 1 && ga->getAvgFitness()[0] > gaList[i-1]->getAvgFitness()[0]){
                            terminateGAs(i - 1);
                            lowestActiveGAIdx = i;
                        }
                        
                        // If this is a MO problem, check if this GA's first front dominates at least (X % + 1) solutions in the previous GA. If so, terminate previous GA and all before.
                        float percentageRequired = 0.5;
                        if (ga->fitFunc_ptr->numObjectives > 1 && MOterminationCondition(ga, gaList[i-1], percentageRequired)){
//                            cout << "GA with " << ga->populationSize << " popsize dominates GA with " << gaList[i-1]->populationSize << " popsize." << endl;
                            terminateGAs(i - 1);
                            lowestActiveGAIdx = i;
                        }
                        
                    // Else if this GA had the highest population size and is terminated
                    } else if (i == maxPopSizeLevel - 1 && ga->terminated){
                        done = true;
                        output["stoppingCondition"] = "terminated";
                        break;
                    }

                    // If this GA has run 4 times, make sure the next GA also does a run
                    if ((gaList[i]->roundsCount) % interval == 0){
                        whichShouldRun[i+1] = 1;
                        highestActiveGAIdx = min(max(highestActiveGAIdx, i+1), maxPopSizeLevel - 1);
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
    FitnessFunction *fit = gaList[0]->fitFunc_ptr;
    output["evaluations"] = fit->totalEvaluations;
    output["uniqueEvaluations"] =fit->totalUniqueEvaluations;
    output["transformedUniqueEvaluations"] = fit->totalTransformedUniqueEvaluations;
    output["paretoDistance"] = fit->paretoDistanceToJSON();
//    output["elitistArchiveHistory"] = fit->elitistArchiveHistoryToJSON();  
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

// Checks the conditions for termination. Terminate previous GA if either:
// 1 - The amount of solutions in the best front of previous GA that are dominated by solutions in current GA is more than ([percentageRequired] * #solutionsInPreviousGABestFront + 1).
// 2 - All solutions in the best front of previous GA are either dominated or appear in the best front of current GA.
bool RoundSchedule::MOterminationCondition(GA* gaCurrent, GA* gaPrev, float percentageRequired){
    
    int dominationCount = 0;
    int neededDominations = floor(percentageRequired * gaPrev->sortedPopulation[0].size()) + 1;
    int subsetCount = 0;
    
    for (Individual* indPrev : gaPrev->sortedPopulation[0]){
        for (Individual* indCur : gaCurrent->sortedPopulation[0]){
            // The set of dominated solutions and the set of solutions that are also in current GA's best front are disjoint.
            if (indCur->dominates(*indPrev)){
                dominationCount++;
                break;
            }
            
            if (indCur->fitnessEquals(*indPrev)){
                subsetCount++;
                break;
            }
        }
        // If the required amount of solutions is dominated, terminate previous GA.
        if (dominationCount >= neededDominations){
            return true;
        }
    }
    // If all solutions in previous GA's best front are dominated or are in the best front of current GA, then terminate previous GA.
    if (subsetCount + dominationCount == gaPrev->sortedPopulation[0].size()){
        return true;
    }
    return false;
}


int RoundSchedule::getAmountOfEvaluations(){
    int evaluations = 0;
    for (GA *ga : gaList) {
        evaluations += ga->getTotalAmountOfEvaluations();
    }
    return evaluations;
}

bool RoundSchedule::maxEvaluationsExceeded() {
    return gaList[0]->fitFunc_ptr->maxEvaluationsExceeded();
}

bool RoundSchedule::maxUniqueEvaluationsExceeded() {
    return gaList[0]->fitFunc_ptr->maxUniqueEvaluationsExceeded();
}

void RoundSchedule::writeOutputGenerationCSV(string filename){
    std::ofstream myfile;
    myfile.open(filename);
    for (int i = 0; i < ga->population.size(); i++){
        myfile << ga->population[i].toString(ga->population[i].genotype);
        for (int j = 0; j < ga->fitFunc_ptr->numObjectives; j++){
            myfile << "," << to_string(ga->population[i].fitness[j]);
        }
        myfile << "\n";
    }
    myfile.close();
}
