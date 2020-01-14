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
#include "ARK3.hpp"
#include "ARK4.hpp"
#include "Utility.hpp"
#include "RoundSchedule.hpp"
#include "GA.hpp"
#include "GOM.hpp"
#include "GOM_LS.hpp"
#include "SimpleGA.hpp"
#include "RandomSearch.hpp"
#include "LocalSearch.hpp"
#include "LocalSearchStochastic.hpp"
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
    writeRawData(main_json.dump(), dataDir);
}

void runNasbench(){

    // Set to -1 if it should not be a stopping condition
    int maxRounds = -1;
    int maxSeconds = -1;
    int maxPopSizeLevel = 500;
    int maxEvaluations = -1; //10000
    int interval = 4;
    int repetitions = 2; //100
    bool nonIMS = false;
    
    //Determines whether convergence data should be saved (which results in increased output file sizes)
    bool scalabilityExperiment = true;
    
    int minProblemSize = 12;
    int maxProblemSize = 12;
    
    for (int problemSize = minProblemSize; problemSize <= maxProblemSize; problemSize++){
        cout << "PROBLEMSIZE " << problemSize << endl;
        
        json main_json;
        main_json["maxPopSizeLevel"] = maxPopSizeLevel;
        main_json["maxRounds"] = maxRounds;
        main_json["maxSeconds"] = maxSeconds;
        main_json["maxEvaluations"] = maxEvaluations;
        main_json["repetitions"] = repetitions;
        main_json["interleavedRoundInterval"] = interval;
        
        bool allowIdentityLayers = true;
        FitnessFunction * fit = new ARK2(problemSize, allowIdentityLayers, maxEvaluations);
//        FitnessFunction * fit = new ARK3(maxEvaluations);
        main_json["fitnessFunction"] = fit->id();
        main_json["optimum"] = fit->optimum;
        
        bool forcedImprovement = true;
        vector<GA*> gaList = {
//            new GOM(fit, new IncrementalLTReversed_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLTReversed_Univariate_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLTReversed_UnivariateOrdered_FOS(), forcedImprovement),
//            new GOM(fit, new LearnedLT_FOS(fit->problemType), forcedImprovement),
//            new GOM(fit, new Univariate_FOS(), forcedImprovement),
//            new GOM(fit, new UnivariateOrdered_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLT_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLT_UnivariateOrdered_FOS(), forcedImprovement),
            new GOM(fit, new UnivariateOrderedReversed_FOS(), forcedImprovement),
//            new GOM(fit, new Triplet_FOS(Utility::Order::ASCENDING), forcedImprovement),
//            new GOM(fit, new Triplet_FOS(Utility::Order::DESCENDING), forcedImprovement),
//            new GOM(fit, new Triplet_FOS(Utility::Order::RANDOM), forcedImprovement),
//            new GOM(fit, new TripletTree_FOS(Utility::Order::ASCENDING), forcedImprovement),
//            new GOM(fit, new TripletTree_FOS(Utility::Order::DESCENDING), forcedImprovement),
//            new GOM(fit, new TripletTree_FOS(Utility::Order::RANDOM), forcedImprovement),
            new GOM_LS(fit, new LearnedLT_FOS(fit->problemType), new LocalSearch(fit, Utility::Order::RANDOM), forcedImprovement),
//            new GOM_LS(fit, new IncrementalLT_UnivariateOrdered_FOS(), new LocalSearch(fit, Utility::Order::RANDOM), forcedImprovement),
            new RandomSearch(fit),
            new SimpleGA(fit, new UnivariateCrossover(), new TournamentSelection(2)),
//            new SimpleGA(fit, new OnePointCrossover(), new TournamentSelection(2)),
            new LocalSearch(fit, Utility::Order::RANDOM),
//            new LocalSearch(fit, Utility::Order::ASCENDING),
//            new LocalSearch(fit, Utility::Order::DESCENDING),
            new LocalSearchStochastic(fit, Utility::Order::RANDOM, 0.01),
//            new LocalSearchStochastic(fit, Utility::Order::RANDOM, 0.05),
//            new LocalSearchStochastic(fit, Utility::Order::RANDOM, 0.1),
//            new LocalSearchStochastic(fit, Utility::Order::RANDOM, 0.2),
        };
        
        
        json experiments;
        for(GA* ga : gaList){
            string gaID = ga->id();
            int populationSize = -1;
            if (nonIMS){
                populationSize = ga->findMinimallyNeededPopulationSize(100, 99);
                cout << "Needed popsize = " << populationSize << endl;
                gaID += ("_FixedPop");
            }
            
            bool exceeded = false;
            json setting;
            for(int rep = 0; rep < repetitions; rep++){
                RoundSchedule rs(maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, interval);
                ga->fitFunc_ptr->clear();
                rs.initialize(ga, problemSize, nonIMS, populationSize);
                json result = rs.run();
                setting[to_string(rep)] = result;
                cout << "rep" << padWithSpacesAfter(to_string(rep), 2)
                << " ga=" << gaID
                << " l=" << problemSize
                << " success=" << padWithSpacesAfter(to_string(result.at("success")), 5)
                << " time=" << padWithSpacesAfter(to_string(result.at("timeTaken")), 6)
                << " evals=" << padWithSpacesAfter(to_string(result.at("evaluations")), 7)
                << " uniqEvals=" << padWithSpacesAfter(to_string(result.at("uniqueEvaluations")), 7)
                << " trUniqEvals=" << padWithSpacesAfter(to_string(result.at("transformedUniqueEvaluations")), 6) << endl;
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
                experiments[gaID] = prob_json;
            }
        }
        
        main_json["experiments"] = experiments;
        write(main_json.dump(), dataDir);
    }
}


int main(int argc, const char * argv[]) {
    
    runNasbench();
//    roundSchedule();
    
//    ARK* fitfunc = new ARK2(13, false, -1);
//    fitfunc->setLength(13);
    
//    int runs = 100000;
//    LocalSearchAnalysis::localSearchTests(fitfunc, runs, "ascending");
//    LocalSearchAnalysis::localSearchTests(fitfunc, runs, "descending");
//    LocalSearchAnalysis::localSearchTests(fitfunc, runs, "random");
    
    
//    GreedyAnalysis::greedyRunBackward();
//    GreedyAnalysis::greedyRun();
//    GreedyAnalysis::greedyBothWays();
//    GreedyAnalysis::greedyInsideOut();
//    for(int i = 12; i <= 12; i++){
//        GreedyAnalysis::findBest(i, 2);
//        cout << endl;
//    }
//    pair<int,int> values12 = GreedyAnalysis::findAmountOfArchitecturesWithFitnessAboveThreshold(12, 3, (91.44-0.001));
//    cout << values12.first << "/" << values12.second << endl;
//
//    pair<int,int> values13 = GreedyAnalysis::findAmountOfArchitecturesWithFitnessAboveThreshold(12, 2, (92.25-0.001));
//    cout << values13.first << "/" << values13.second << endl;
    
//    pair<int,int> values10 = GreedyAnalysis::findAmountOfArchitecturesWithFitnessAboveThreshold(5, 3, (89.26-0.001));
//    cout << values10.first << "/" << values10.second << endl;
    
    return 0;
}


