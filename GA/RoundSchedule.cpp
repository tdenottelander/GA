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

RoundSchedule::RoundSchedule (int maxRounds, int maxLevel, int maxSeconds, int interleavedRoundInterval) :
    maxRounds(maxRounds),
    maxLevel(maxLevel),
    maxSeconds(maxSeconds),
    interval(interleavedRoundInterval)
{}

void RoundSchedule::initialize(Selection &sel, Variation &var, int problemSize) {
    gaList.reserve(maxLevel);
    selection = &sel;
    variation = &var;
    
    //TODO: Make this modular:
    output["fitness"] = "onemax";
    output["successfulGAPopulation"] = -1;
    output["successfulGARoundCount"] = -1;
    output["success"] = false;
    output["stoppingCondition"] = "-1";
    
    //TODO: Refactor as armadillo uvec
    whichShouldRun.reserve(maxLevel);
    for(int i = 0; i < maxLevel; i++){
        whichShouldRun.push_back(0);
        int popSize = pow(2, i + 1);
        GA* ga = new GA(popSize, problemSize, new OneMax(problemSize), selection, variation);
        gaList.push_back(ga);
    }
    whichShouldRun[0] = 1;
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
//            cout << "Did not found the optimum after " << round << " rounds" << endl;
            output["stoppingCondition"] = "maxRoundsExceeded";
            break;
        } else if (maxSeconds != -1 && millis() - start > maxSeconds * 1000) {
//            cout << "Did not found the optimum after " << maxSeconds * 1000 << " seconds" << endl;
            output["stoppingCondition"] = "maxTimeExceeded";
            break;
        }

        for (int i = lowestActiveGAIdx; i < maxLevel; i++) {

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
//                        cout << "Init GA(" << ga->populationSize << ")" << endl;
                        ga->initialize();
                    }

                    // Do the round on this GA
//                    cout << "     GA(" << ga->populationSize << ") round " << ga->roundsCount << endl;
                    ga->round();

                    // If the current GA has found the optimum, break out of the loop
                    if (ga->isOptimal()){
//                        cout << "Opt  GA(" << ga->populationSize << ") at round " << ga->roundsCount << endl;
                        optimumFound = true;
                        output["successfulGAPopulation"] = ga->populationSize;
                        output["successfulGARoundCount"] = ga->roundsCount;
                        break;

                        // Else if the GA is converged, terminate this GA and all before.
                    } else if (ga->isConverged()){
                        // GA.termination is already set to true inside isConverged()-method
                        // Set all previous GA's to terminated
//                        cout << "Term GA(" << ga->populationSize << ") and all before" << endl;
                        terminateGAs(i);
                        lowestActiveGAIdx = i + 1;
                        continue;

                        // Else if it has a higher fitness than the previous GA, terminate all before this one
                    } else if (i != 0 && !gaList[i-1]->terminated && ga->getAvgFitness() > gaList[i-1]->getAvgFitness()){
//                        cout << "Term GA(" << gaList[i-1]->populationSize << ") and all before" << endl;
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
    
    return output;
}

// Terminate ga's in gaList up to and including index n
void RoundSchedule::terminateGAs(int n){
    for(int i = 0; i < (n + 1); i++){
        (*gaList[i]).terminated = true;
    }
}
