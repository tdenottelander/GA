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
#include <nlohmann/json.hpp>
#include "Individual.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "FitnessFunction.hpp"
#include "GA.hpp"
#include "Utility.hpp"
#include "RoundSchedule.hpp"

using namespace std;
using namespace arma;
using namespace Utility;
using nlohmann::json;

// The random generator used throughout the whole application
mt19937 rng(1234);
// A distribution that is used to get values between 0.0 and 1.0 by use of the rng defined above
uniform_real_distribution<float> dist(0.0, 1.0);

const string dataDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/data/";

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

            if(ga.fitFunc_ptr->optimumFound){
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
    
    long end = millis();
    cout << "Time taken: " << end - begin << "ms" << endl;
    
    return 0;
}

void roundSchedule(){
    RoundSchedule rs(500, 10);
    TournamentSelection sel(4);
    UnivariateCrossover var;
    int problemSize = 50;
    rs.initialize(sel, var, problemSize);
    json j = rs.run();
    cout << j.dump() << endl;
    
    ofstream out("test.txt", ofstream::out);
    out << j.dump();
    out.close();
    
    write(j.dump(), dataDir);
}

int main(int argc, const char * argv[]) {
    
    roundSchedule();
//    mainLoop();
    
    return 0;
}
