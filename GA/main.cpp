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
#include "SimpleMOProblem.hpp"
#include "CountingOnesMO.hpp"
#include "ZerosOnes.hpp"
#include "LOTZ.hpp"
#include "NK.hpp"
#include "ARK.hpp"
#include "ARK1.hpp"
#include "ARK2.hpp"
#include "ARK3.hpp"
#include "ARK4.hpp"
#include "ARK5.hpp"
#include "ARK6.hpp"
#include "ARK7.hpp"
#include "Utility.hpp"
#include "RoundSchedule.hpp"
#include "GA.hpp"
#include "GOM.hpp"
#include "GOM_LS.hpp"
#include "SimpleGA.hpp"
#include "RandomSearch.hpp"
#include "LocalSearch.hpp"
#include "LocalSearchStochastic.hpp"
#include "NSGA_II.hpp"
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
bool storeAbsoluteConvergence = false;
bool storeUniqueConvergence = true;
bool storeTransformedUniqueConvergence = false;
std::string ARK_Analysis_suffix = "";

void runNasbench(){

    // Set to -1 if it should not be a stopping condition
    int maxRounds = -1;
    int maxSeconds = -1;
    int maxPopSizeLevel = 500;
    int maxEvaluations = 25000; //10000
    int maxUniqueEvaluations = -1;
    int interval = 4;
    int repetitions = 20; //100
    bool IMS = true;
    int populationSize = 20;
    
    int minProblemSize = 10;
    int maxProblemSize = 10;
    
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
        bool genotypeChecking = true;
//        FitnessFunction * fit = new ARK2(problemSize, allowIdentityLayers, genotypeChecking);
//        FitnessFunction * fit = new ARK5(problemSize, allowIdentityLayers);
    
//        FitnessFunction * fit = new ARK6(problemSize, genotypeChecking);
//        ARK * fit = new ARK7(problemSize, genotypeChecking);
//        fit->setNoisy(0.01);
        
//        FitnessFunction * fit = new ARK3();
        
//        FitnessFunction * fit = new Trap(5, 5);
//        FitnessFunction * fit = new OneMax(20);
//        FitnessFunction * fit = new LeadingOnes(20);
//        FitnessFunction * fit = new SimpleMOProblem(4, 2);
//        FitnessFunction * fit = new CountingOnesMO(problemSize,2);
//        FitnessFunction * fit = new ZerosOnes(problemSize);
        FitnessFunction * fit = new LOTZ(problemSize);
        
//        int blocksize = 5;
//        int alphabetsize = 2;
//        FitnessFunction * fit = new NK(problemSize, blocksize, false, alphabetsize);
        
        main_json["fitnessFunction"] = fit->id();
        main_json["optimum"] = fit->optimum;
        main_json["alphabet"] = fit->problemType->id();
        
        bool forcedImprovement = true;
        vector<GA*> gaList = {
//            new GOM(fit, new Univariate_FOS(Utility::Order::RANDOM), forcedImprovement),
//            new GOM(fit, new Univariate_FOS(Utility::Order::ASCENDING), forcedImprovement),
//            new GOM(fit, new Univariate_FOS(Utility::Order::DESCENDING), forcedImprovement),

//            new GOM(fit, new IncrementalLTReversed_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLTReversed_Univariate_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLTReversed_UnivariateOrdered_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLT_UnivariateOrdered_FOS(), forcedImprovement),
//            new GOM(fit, new IncrementalLT_FOS(), forcedImprovement),

//            new GOM(fit, new Triplet_FOS(Utility::Order::ASCENDING), forcedImprovement),
//            new GOM(fit, new Triplet_FOS(Utility::Order::DESCENDING), forcedImprovement),
//            new GOM(fit, new Triplet_FOS(Utility::Order::RANDOM), forcedImprovement),
//            new GOM(fit, new TripletTree_FOS(Utility::Order::ASCENDING), forcedImprovement),
//            new GOM(fit, new TripletTree_FOS(Utility::Order::DESCENDING), forcedImprovement),
//            new GOM(fit, new TripletTree_FOS(Utility::Order::RANDOM), forcedImprovement),

//            new GOM(fit, new ARK6_FOS(Utility::Order::ASCENDING), forcedImprovement),
//            new GOM(fit, new ARK6_FOS(Utility::Order::DESCENDING), forcedImprovement),
//            new GOM(fit, new ARK6_FOS(Utility::Order::RANDOM), forcedImprovement),

//            new GOM(fit, new LearnedLT_FOS(fit->problemType), forcedImprovement),

//            new GOM_LS(fit, new LearnedLT_FOS(fit->problemType), new LocalSearch(fit, Utility::Order::RANDOM), forcedImprovement),
//            new GOM_LS(fit, new IncrementalLT_UnivariateOrdered_FOS(), new LocalSearch(fit, Utility::Order::RANDOM), forcedImprovement),
//
//            new RandomSearch(fit),

//            new SimpleGA(fit, new UnivariateCrossover(), new TournamentSelection(2)),
//            new SimpleGA(fit, new OnePointCrossover(), new TournamentSelection(2)),
//
//            new LocalSearch(fit, Utility::Order::RANDOM),
//            new LocalSearch(fit, Utility::Order::ASCENDING),
//            new LocalSearch(fit, Utility::Order::DESCENDING),

//            new LocalSearchStochastic(fit, Utility::Order::RANDOM, 0.01),
//            new LocalSearchStochastic(fit, Utility::Order::RANDOM, 0.05),
            
            new NSGA_II(fit, new OnePointCrossover(), new TournamentSelection(2), true),
        };
        
        
        json experiments;
        for(GA* ga : gaList){
            string gaID = ga->id();
//            cout << gaID << endl;
            if (!IMS){
                if (populationSize < 0){
                    populationSize = ga->findMinimallyNeededPopulationSize(100, 99);
                    cout << "Needed popsize = " << populationSize << endl;
                }
                gaID += ("_FixedPop");
            }
            
            bool exceeded = false;
            json setting;
            for(int rep = 0; rep < repetitions; rep++){
                RoundSchedule rs(maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, maxUniqueEvaluations, interval);
                ga->fitFunc_ptr->clear();
                rs.initialize(ga, problemSize, IMS, populationSize);
                json result = rs.run();
                rs.writeOutputGenerationCSV(dataDir + "outputgen.csv");
                setting[to_string(rep)] = result;
                cout << "rep" << padWithSpacesAfter(to_string(rep), 2)
                << " ga=" << gaID
                << " l=" << problemSize
                << " success=" << padWithSpacesAfter(to_string(result.at("success")), 5)
                << " time=" << padWithSpacesAfter(to_string(result.at("timeTaken")), 12)
                << " evals=" << padWithSpacesAfter(to_string(result.at("evaluations")), 15)
                << " uniqEvals=" << padWithSpacesAfter(to_string(result.at("uniqueEvaluations")), 15)
                << " trUniqEvals=" << padWithSpacesAfter(to_string(result.at("transformedUniqueEvaluations")), 15) << endl;
                if(result.at("stoppingCondition") == "maxTimeExceeded"){
                    cout << "Max time exceeded, not starting anymore runs" << endl;
                    break;
                } else if(result.at("stoppingCondition") == "maxEvaluationsExceeded"){
//                    exceeded = true;
//                    cout << "Max evaluations exceeded";
//                    if (storeConvergence){
//                        cout << ", not starting anymore runs" << endl;;
//                        break;
//                    } else
//                        cout << endl;
                }
            }
            cout << endl;
            if(storeConvergence && exceeded){
                continue;
            } else {
                json prob_json;
                prob_json[to_string(problemSize)] = setting;
                experiments[gaID] = prob_json;
                
                // Extra write inbetween algorithms
//                main_json["experiments"] = experiments;
//                writeRawData(main_json.dump(), dataDir);
            }
        }
        
        main_json["experiments"] = experiments;
        writeRawData(main_json.dump(), dataDir);
    }
}

int main(int argc, const char * argv[]) {
    
    runNasbench();

//    ARK * ark = new ARK6(0, -1);
//    ark->doAnalysis(0, 14);
    
    return 0;
}


