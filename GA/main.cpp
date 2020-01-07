//
//  main.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "GreedyAnalysis.hpp"

#include <random>
#include <iostream>
#include <armadillo>
#include <nlohmann/json.hpp>
#include "Individual.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "FitnessFunction.hpp"
#include "Trap.hpp"
#include "ARK.hpp"
#include "ARK1.hpp"
#include "ARK2.hpp"
#include "Utility.hpp"
#include "RoundSchedule.hpp"
#include "GA.hpp"
#include "GOM.hpp"
#include "SimpleGA.hpp"
#include "RandomSearch.hpp"
#include "ProblemType.hpp"
#include "LearnedLTFOS.hpp"
#include <stdlib.h>

#include "LocalSearchAnalysis.hpp"

using namespace std;
using namespace arma;
using namespace Utility;
using nlohmann::json;

// The random generator used throughout the whole application
mt19937 rng(1234);
// A distribution that is used to get values between 0.0 and 1.0 by use of the rng defined above
uniform_real_distribution<float> dist(0.0, 0.9999);

const string projectsDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/";
const string dataDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/data/";

bool printfos = false;
bool printPopulationAfterRound = false;
bool printPopulationOnOptimum = false;
bool storeConvergence = false;
bool storeUniqueConvergence = true;
bool storeTransformedUniqueConvergence = true;

void roundSchedule(){
    json main_json;

    // Set to -1 if it should not be a stopping condition
    int maxRounds = -1;
    int maxSeconds = -1;
    int maxPopSizeLevel = 100;
    int maxEvaluations = 1000000;
    int interval = 4;
    int repetitions = 10;
    int maxProblemExponent = 10;
    
    main_json["maxPopSizeLevel"] = maxPopSizeLevel;
    main_json["maxRounds"] = maxRounds;
    main_json["maxSeconds"] = maxSeconds;
    main_json["maxEvaluations"] = maxEvaluations;
    main_json["repetitions"] = repetitions;
    main_json["maxProblemExponent"] = maxProblemExponent;
    main_json["interleavedRoundInterval"] = interval;
    
    FitnessFunction * fit = new Trap(5, maxEvaluations);
    main_json["fitnessFunction"] = fit->id();
    
    vector<GA*> gaList = {
        new GOM(fit, new LearnedLT_FOS(fit->problemType), true),
        new SimpleGA(fit, new UnivariateCrossover(), new TournamentSelection(2)),
        new SimpleGA(fit, new OnePointCrossover(), new TournamentSelection(2)),
        new GOM(fit, new Univariate_FOS(), true),
        new GOM(fit, new UnivariateOrdered_FOS(), true),
        new GOM(fit, new IncrementalLT_FOS(), true),
        new GOM(fit, new IncrementalLT_UnivariateOrdered_FOS(), true)
    };
    
    json experiments;
    bool breakOutOfProblemSize = false;
    for(GA* ga : gaList){
        json setting;
        for(int i = 1; i < maxProblemExponent; i++){
            int problemSize = pow(2,i);
            json prob_json;
            for(int rep = 0; rep < repetitions; rep++){
                RoundSchedule rs(maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, interval);
                rs.initialize(ga, problemSize);
                json result = rs.run();
                prob_json[to_string(rep)] = result;
                cout << "rep" << rep
                << " ga=" << ga->id()
                << " l=" << problemSize
                << " success=" << result.at("success")
                << " time=" << result.at("timeTaken")
                << " evaluations=" << result.at("evaluations") << endl;
                if(result.at("stoppingCondition") == "maxTimeExceeded"){
                    cout << "Max time exceeded, not starting anymore runs" << endl;
                    breakOutOfProblemSize = true;
                    break;
                } else if(result.at("stoppingCondition") == "maxEvaluationsExceeded"){
                    cout << "Max evaluations exceeded, not starting anymore runs" << endl;
                    breakOutOfProblemSize = true;
                    break;
                }
            }
            cout << endl;
            if(breakOutOfProblemSize){
                breakOutOfProblemSize = false;
                break;
            } else {
                setting[to_string(problemSize)] = prob_json;
            }
        }
        experiments[ga->id()] = setting;
    }
    
    main_json["experiments"] = experiments;
    write(main_json.dump(), dataDir);
}

void runNasbench(){
    
    for (int problemSize = 2; problemSize < 12; problemSize++){
        cout << "PROBLEMSIZE " << problemSize << endl;
        json main_json;
        
        // Set to -1 if it should not be a stopping condition
        int maxRounds = -1;
        int maxSeconds = -1;
        int maxPopSizeLevel = 500;
        int maxEvaluations = -1; //10000
        int interval = 4;
        int repetitions = 100; //100
        
        bool scalabilityExperiment = true;
        
        
        main_json["maxPopSizeLevel"] = maxPopSizeLevel;
        main_json["maxRounds"] = maxRounds;
        main_json["maxSeconds"] = maxSeconds;
        main_json["maxEvaluations"] = maxEvaluations;
        main_json["repetitions"] = repetitions;
        main_json["interleavedRoundInterval"] = interval;
        
        bool allowIdentityLayers = true;
        FitnessFunction * fit = new ARK2(problemSize, allowIdentityLayers, maxEvaluations);
        main_json["fitnessFunction"] = fit->id();
        main_json["optimum"] = fit->optimum;
        
        bool forcedImprovement = true;
        vector<GA*> gaList = {
            new GOM(fit, new IncrementalLTReversed_FOS(), forcedImprovement),
            new GOM(fit, new IncrementalLTReversed_Univariate_FOS(), forcedImprovement),
            new GOM(fit, new IncrementalLTReversed_UnivariateOrdered_FOS(), forcedImprovement),
            new RandomSearch(fit),
            new GOM(fit, new LearnedLT_FOS(fit->problemType), forcedImprovement),
            new SimpleGA(fit, new UnivariateCrossover(), new TournamentSelection(2)),
            new SimpleGA(fit, new OnePointCrossover(), new TournamentSelection(2)),
            new GOM(fit, new Univariate_FOS(), forcedImprovement),
            new GOM(fit, new UnivariateOrdered_FOS(), forcedImprovement),
            new GOM(fit, new IncrementalLT_FOS(), forcedImprovement),
            new GOM(fit, new IncrementalLT_UnivariateOrdered_FOS(), forcedImprovement),
            new GOM(fit, new UnivariateOrderedReversed_FOS(), forcedImprovement)
        };
        
        
        json experiments;
        for(GA* ga : gaList){
            bool exceeded = false;
            json setting;
            for(int rep = 0; rep < repetitions; rep++){
                RoundSchedule rs(maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, interval);
                rs.initialize(ga, problemSize);
                json result = rs.run();
                setting[to_string(rep)] = result;
                cout << "rep" << rep
                << " ga=" << ga->id()
                << " l=" << problemSize
                << " success=" << result.at("success")
                << " time=" << result.at("timeTaken")
                << " evals=" << result.at("evaluations")
                << " uniqEvals=" << result.at("uniqueEvaluations")
                << " trUniqEvals=" << result.at("transformedUniqueEvaluations") << endl;
                if(result.at("stoppingCondition") == "maxTimeExceeded"){
                    cout << "Max time exceeded, not starting anymore runs" << endl;
                    break;
                } else if(result.at("stoppingCondition") == "maxEvaluationsExceeded"){
                    exceeded = true;
                    cout << "Max evaluations exceeded";
                    if (scalabilityExperiment){
                        cout << ", not starting anymore runs" << endl;;
                        break;
                    } else
                        cout << endl;
                }
            }
            cout << endl;
            if(scalabilityExperiment && exceeded){
                continue;
            } else {
                json prob_json;
                prob_json[to_string(problemSize)] = setting;
                experiments[ga->id()] = prob_json;
            }
        }
        
        main_json["experiments"] = experiments;
        write(main_json.dump(), dataDir);
    }
}


int main(int argc, const char * argv[]) {
    
//    runNasbench();
//    roundSchedule();
    
    ARK* fitfunc = new ARK2(13, false, -1);
//    fitfunc->setLength(13);
    
    int runs = 10000;
    LocalSearchAnalysis::localSearchTests(fitfunc, runs, "ascending");
    LocalSearchAnalysis::localSearchTests(fitfunc, runs, "descending");
    LocalSearchAnalysis::localSearchTests(fitfunc, runs, "random");
    
//    LocalSearchAnalysis::localSearchTests(1000, "random");
    
    
//    GreedyAnalysis::greedyRunBackward();
//    GreedyAnalysis::greedyRun();
//    GreedyAnalysis::greedyBothWays();
//    GreedyAnalysis::greedyInsideOut();
//    for(int i = 13; i < 14; i++){
//        GreedyAnalysis::findBest(i, 2);
//        cout << endl;
//    }
    
    return 0;
}


