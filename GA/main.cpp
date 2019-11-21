//
//  main.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include <random>
#include <iostream>
#include <armadillo>
#include "Individual.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "FitnessFunction.hpp"
#include "GA.hpp"
#include "Utility.hpp"

using namespace std;
using namespace arma;
using namespace Utility;

// The random generator used throughout the whole application
mt19937 rng(1234);
// A distribution that is used to get values between 0.0 and 1.0 by use of the rng defined above
uniform_real_distribution<float> dist(0.0, 1.0);

int mainLoop(){
    long begin = millis();

    int l = 100;
    int n = 150;
    int maxGenerations = 50;
    int runs = 100;
    
    int optFoundCounter = 0;
    
    for (int run = 0; run < runs; run++){
        bool optFound = false;
        cout << "run " << run+1 << ". ";
        
        TournamentSelection selection(4);
        UnivariateCrossover variation;
        OneMax fitnessFunc(l);
        
        GA ga (n, l, &fitnessFunc, &selection, &variation);
        ga.initialize();
//        cout << ga.toString() << endl;
        
        for(int gen = 0; gen < maxGenerations; gen++){
            ga.roundPOVariationSelection();
//            cout << "\nRound " << j << "  | Avg fitness: " << ga.getAvgFitness() << "  | Best fitness: " << ga.fitFunc->bestIndividual.fitness << endl;
//            cout << ga.toString() << endl;
//            cout << "converged " << ga.isConverged() << endl;
//            cout << "diverse " << ga.isDiverse() << endl;

            if(ga.fitFunc->optimumFound){
                optFound = true;
                cout << "Optimum found after " << gen << " generations" << endl;
//                cout << "Individual: " << ga.fitFunc->bestIndividual.toString() << endl;
                break;
            }
        }
        
        if(!optFound){
            cout << "No optimum found" << endl;
        } else {
            optFoundCounter++;
        }
    }
    
    cout << "Optimal found " << optFoundCounter << " out of " << runs << " runs" << endl;
    
//    auto x = randu(1);
//    double x = arma::arma_rng::randu<double>();
//    mat A = randu<mat>(4,5);
//    vec  v = ones<vec>(10);
//    vector<int> x = Utility::getRandomlyPermutedArray(5);

//    cout << x[0] << endl;
    
    long end = millis();
    cout << "Time taken: " << end - begin << "ms" << endl;
    
    return 0;
}

// Terminate ga's in gaList up to and including index n
void terminateRuns(vector<GA*> &gaList, int n){
    for(int i = 0; i < (n + 1); i++){
        (*gaList[i]).terminated = true;
    }
}

void interleavedRoundsSchedule (int maxLevel, int maxRounds) {
    vector<GA*> gaList;
    gaList.reserve(maxLevel);
    TournamentSelection selection(4);
    UnivariateCrossover variation;
    int l = 50;
    int k = 4;
    
    //TODO: Refactor as armadillo uvec
    vector<int> whichShouldRun;
    whichShouldRun.reserve(maxLevel);
    for(int i = 0; i < maxLevel; i++){
        whichShouldRun.push_back(0);
        OneMax fitnessFunc(l);
        int popSize = pow(2, i + 1);
//        GA ga(popSize, l, new OneMax(l), &selection, &variation);
        gaList.push_back(new GA(popSize, l, new OneMax(l), &selection, &variation));
    }
    
    whichShouldRun[0] = 1;
    
    int round = 0;
    int lowestActiveGAIdx = 0;
    bool optimumFound = false;
    bool done = false;
    while (!done) {
        
        //Stopping conditions
        if (round >= maxRounds) {
            cout << "Did not found the optimum after " << round << " rounds" << endl;
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
                        cout << "Init GA(" << ga->populationSize << ")" << endl;
                        ga->initialize();
                    }
                    
                    // Do the round on this GA
                    cout << "     GA(" << ga->populationSize << ") round " << ga->roundsCount << endl;
                    ga->round();
                    
                    // If the current GA has found the optimum, break out of the loop
                    if (ga->isOptimal()){
                        cout << "Opt  GA(" << ga->populationSize << ") at round " << ga->roundsCount << endl;
                        optimumFound = true;
                        break;
                    
                    // Else if the GA is converged, terminate this GA and all before.
                    } else if (ga->isConverged()){
                        // GA.termination is already set to true inside isConverged()-method
                        // Set all previous GA's to terminated
                        cout << "Term GA(" << ga->populationSize << ") and all before" << endl;
                        terminateRuns(gaList, i);
                        lowestActiveGAIdx = i + 1;
                        continue;
                        
                    // Else if it has a higher fitness than the previous GA, terminate all before this one
                    } else if (!gaList[i-1]->terminated && i != 0 && ga->getAvgFitness() > gaList[i-1]->getAvgFitness()){
                        cout << "Term GA(" << gaList[i-1]->populationSize << ") and all before" << endl;
                        terminateRuns(gaList, i - 1);
                        lowestActiveGAIdx = i;
                        continue;
                    }
                    
                    // If this GA has run 4 times, make sure the next GA also does a run
                    if ((gaList[i]->roundsCount) % k == 0){
                        whichShouldRun[i+1] = 1;
                    }
                }
            }
        }
        
        if(optimumFound){
            break;
        }
        
        //Increment counter
        round++;
    }
}

int main(int argc, const char * argv[]) {
    
    interleavedRoundsSchedule(10, 500);
//    mainLoop();
    
//    auto x = randu(1);
//        double x = arma::randu();
    //    mat A = randu<mat>(4,5);
    //    vec  v = ones<vec>(10);
    //    vector<int> x = Utility::getRandomlyPermutedArray(5);
    
//        cout << x << endl;
    
    return 0;
}
