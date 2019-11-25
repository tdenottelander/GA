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
uniform_real_distribution<float> dist(0.0, 0.9999);

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
    json experiment;

    // Set to -1 if it should not be a stopping condition
    int maxRounds = -1;
    int maxSeconds = 60;
    int maxLevel = 100;
    int interval = 4;
    int repetitions = 30;
    int maxProblemExponent = 100;
    
    experiment["maxLevel"] = maxLevel;
    experiment["maxRounds"] = maxRounds;
    experiment["maxSeconds"] = maxSeconds;
    experiment["repetitions"] = repetitions;
    experiment["maxProblemExponent"] = maxProblemExponent;
    experiment["interleavedRoundInterval"] = interval;
    
    vector<Variation*> variations = {new OnePointCrossover(), new UnivariateCrossover()};
    vector<Selection*> selections = {new TournamentSelection(4)};
    
    
    bool breakOutOfProblemSize = false;
    for(Selection *sel : selections){
        json sel_json;
        for(Variation *var : variations){
            json var_json;
            for(int i = 2; i < maxProblemExponent; i++){
                int problemSize = pow(2,i);
                FitnessFunction* fit = new LeadingOnes(problemSize);
                json l_json;
//                l_json["problemSize"] = problemSize;
//                l_json["variation"] = var->id();
//                l_json["selection"] = sel->id();
                for(int rep = 0; rep < repetitions; rep++){
                    RoundSchedule rs(maxRounds, maxLevel, maxSeconds, interval);
                    rs.initialize(*sel, *var, *fit, problemSize);
                    json result = rs.run();
                    l_json[to_string(rep)] = result;
                    cout << "rep" << rep
                    << " Sel=" << sel->id()
                    << " Var=" << var->id()
                    << " l=" << problemSize
                    << " success=" << result.at("success")
                    << " time=" << result.at("timeTaken")
                    << " evaluations=" << result.at("evaluations") << endl;
        //            cout << "Optimum " << (j.at("success") ? "    " : "not ") << "found for l=" << problemSize << " after " << j.at("timeTaken") << "ms" << endl;
                    if(result.at("stoppingCondition") == "maxTimeExceeded"){
                        cout << "Max time exceeded, not starting anymore runs" << endl;
                        breakOutOfProblemSize = true;
                        break;
                    }
                }
                cout << endl;
                if(breakOutOfProblemSize){
                    breakOutOfProblemSize = false;
                    break;
                } else {
                    var_json[to_string(problemSize)] = l_json;
                }
            }
            sel_json["var=" + var->id()] = var_json;
        }
        experiment["sel=" + sel->id()] = sel_json;
    }
    
    write(experiment.dump(), dataDir);

}

int main(int argc, const char * argv[]) {
    
    roundSchedule();
//    mainLoop();
    
    return 0;
}
