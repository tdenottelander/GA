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
#include "ZeroMaxOneMax.hpp"
#include "LOTZ.hpp"
#include "TrapInverseTrap.hpp"
#include "MAXCUT.hpp"
#include "NK.hpp"
#include "ARK.hpp"
#include "ARK1.hpp"
#include "ARK2.hpp"
#include "ARK3.hpp"
#include "ARK4.hpp"
#include "ARK5.hpp"
#include "ARK6.hpp"
#include "ARK7.hpp"
#include "ARK_Online.hpp"
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
#include "MO_LS.hpp"
#include "MO_RS.hpp"
#include "ProblemType.hpp"
#include "LearnedLTFOS.hpp"
#include <stdlib.h>

#include "LocalSearchAnalysis.hpp"

// Include for the original NSGA-II implementation.
//#include "global.h"

// Include for MO-GOMEA
#include "MO_GOMEA.hpp"

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

extern json elitistArchiveJSON;

bool printfos = false;
bool printPopulationAfterRound = false;
bool printPopulationOnOptimum = false;
bool printElitistArchiveOnUpdate = false;
bool storeConvergence = false;
bool storeAbsoluteConvergence = false;
bool storeUniqueConvergence = true;
bool storeTransformedUniqueConvergence = false;
bool storeDistanceToParetoFrontOnElitistArchiveUpdate = true;
bool storeElitistArchive = true;
bool updateElitistArchiveOnEveryEvaluation = true;
int storeParetoDistanceMode = 0; // 0 = on a log10 scale, 1 = linear scale
int storeParetoDistanceLinearInterval = 10;
std::string ARK_Analysis_suffix = "";
FitnessFunction *fit_global;

void runNasbench(){

    // Set to -1 if it should not be a stopping condition
    int maxRounds = -1;
    int maxSeconds = -1;
    int maxPopSizeLevel = 500;
    int maxEvaluations = 155000; //10000
    int maxUniqueEvaluations = -1;
    int interval = 4;
    int repetitions = 20; //100
    bool IMS = false;
    int nonIMSPopsize = 40;
    
    int minProblemSize = 3;
    int maxProblemSize = 3;
    
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
        bool genotypeChecking = false;
//        FitnessFunction * fit = new ARK2(problemSize, allowIdentityLayers, genotypeChecking);
//        FitnessFunction * fit = new ARK5(problemSize, allowIdentityLayers);
    
//        FitnessFunction * fit = new ARK6(problemSize, genotypeChecking);
//        ARK * fit = new ARK7(problemSize, genotypeChecking, true);
//        fit->setNoisy(0.01);
        
//        FitnessFunction * fit = new ARK_Online();
        
//        FitnessFunction * fit = new ARK3();
        
        FitnessFunction * fit = new Trap(5, 5);
//        FitnessFunction * fit = new OneMax(20);
//        FitnessFunction * fit = new LeadingOnes(20);
        
//        FitnessFunction * fit = new ZeroMaxOneMax(problemSize);
//        FitnessFunction * fit = new LOTZ(problemSize);
//        FitnessFunction * fit = new TrapInverseTrap(problemSize);
//        FitnessFunction * fit = new MAXCUT(problemSize);
//        fit->convergenceCriteria = FitnessFunction::ConvergenceCriteria::ENTIRE_PARETO;
//        fit->convergenceCriteria = FitnessFunction::ConvergenceCriteria::EPSILON_PARETO_DISTANCE;
//        fit->epsilon = 0.00001;
        
//        int blocksize = 5;
//        int alphabetsize = 2;
//        FitnessFunction * fit = new NK(problemSize, blocksize, false, alphabetsize);
        
        main_json["fitnessFunction"] = fit->id();
        main_json["optimum"] = fit->optimum;
        main_json["alphabet"] = fit->problemType->id();
        
        bool forcedImprovement = true;
        vector<GA*> gaList = {
            // ------------- SINGLE OBJECTIVE ALGORITHMS -------------
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

            new GOM(fit, new LearnedLT_FOS(fit->problemType), forcedImprovement),

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
            

            // ------------- MULTI OBJECTIVE ALGORITHMS -------------
//            new NSGA_II(fit, new TwoPointCrossover(), 0.9, true),
//            new MO_LS(fit, Utility::Order::RANDOM, 1000000),
//            new MO_RS(fit),
            
//            new NSGA_II(fit, new ARK6_Crossover(), 0.9, true),
//            new NSGA_II(fit, new UnivariateCrossover(), 0.9, true),
//            new NSGA_II(fit, new ThreePointCrossover(), 0.9, true),
//            new NSGA_II(fit, new OnePointCrossover(), 0.9, true)
        };
        
        string outputfileName = dataDir + Utility::getDateString() + "_rawdata.json";
        
        json experiments;
        for(GA* ga : gaList){
            string gaID = ga->id();
            if (!IMS){
                if (nonIMSPopsize < 0){
                    nonIMSPopsize = ga->findMinimallyNeededPopulationSize(100, 99);
                    cout << "Needed popsize = " << nonIMSPopsize << endl;
                }
                gaID += ("_pop=" + to_string(nonIMSPopsize));
            }
            
            bool exceeded = false;
            json setting;
            vector<int> evals;
            vector<int> uniqueEvals;
            vector<int> times;
            for(int rep = 0; rep < repetitions; rep++){
                RoundSchedule rs(maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, maxUniqueEvaluations, interval);
                ga->fitFunc_ptr->clear();
                rs.initialize(ga, problemSize, IMS, nonIMSPopsize);
                json result = rs.run();
//                rs.writeOutputGenerationCSV(dataDir + "outputgen.csv");
//                fit->saveElitistArchiveToJSON();
                setting[to_string(rep)] = result;
                cout << "rep" << padWithSpacesAfter(to_string(rep), 2)
                << " ga=" << gaID
                << " l=" << problemSize
                << " success=" << padWithSpacesAfter(to_string(result.at("success")), 5)
                << " time=" << padWithSpacesAfter(to_string(result.at("timeTaken")), 12)
                << " evals=" << padWithSpacesAfter(to_string(result.at("evaluations")), 15)
                << " uniqEvals=" << padWithSpacesAfter(to_string(result.at("uniqueEvaluations")), 15);
                if(storeTransformedUniqueConvergence) cout << " trUniqEvals=" << padWithSpacesAfter(to_string(result.at("transformedUniqueEvaluations")), 15);
                cout << endl;
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
                times.push_back(result.at("timeTaken"));
                evals.push_back(result.at("evaluations"));
                uniqueEvals.push_back(result.at("uniqueEvaluations"));
            }
            cout << endl;
            
            cout << "Avg Time: " << Utility::getAverage(times) << endl;
            cout << "Avg Evals: " << Utility::getAverage(evals) << endl;
            cout << "Avg Unique Evals: " << Utility::getAverage(uniqueEvals) << endl;
            cout << "Elitist archive:   (size=" << fit->elitistArchive.size() << ")" << endl;
            for (int i = 0; i < fit->elitistArchive.size(); i++){
                cout << i << ": " << fit->elitistArchive[i].toString() << endl;
            }
            if(storeConvergence && exceeded){
                continue;
            } else {
                json prob_json;
                prob_json[to_string(problemSize)] = setting;
                experiments[gaID] = prob_json;
                
                // Extra write inbetween algorithms
                main_json["experiments"] = experiments;
                writeRawData(main_json.dump(), outputfileName);
            }
        }
        
        main_json["experiments"] = experiments;
        writeRawData(main_json.dump(), outputfileName);
    }
}

void run_MO_GOMEA(int argc, const char * argv[]){
    if (argc <= 6){
        MO_GOMEA().main_MO_GOMEA(argc, argv);
        exit(0);
    }
    
    int problemsize = stoi(argv[3]);
    
    string problem = argv[7];
    if (problem == "ark7"){
        fit_global = new ARK7(problemsize, false, true);
    } else if (problem == "zmom"){
        fit_global = new ZeroMaxOneMax(problemsize);
    } else if (problem == "tit"){
        fit_global = new TrapInverseTrap(problemsize);
    } else {
        cout << "Problem not known. Exiting now" << endl;
        exit(0);
    }
    
    int repetitions = 10;
    
    json main;
    for (int i = 0; i < repetitions; i++){
        fit_global->clear();
        MO_GOMEA().main_MO_GOMEA(argc, argv);
        main.push_back(elitistArchiveJSON);
    }
    writeRawData(main.dump(), "elitistArchive.json");
}

int main(int argc, const char * argv[]) {
    
    char mypath[]="PYTHONHOME=/Users/tomdenottelander/miniconda3/envs/nasbench/";
    putenv( mypath );
    
//    runNasbench();
    
    run_MO_GOMEA(argc, argv);
    
//    rng = mt19937(millis());
//    float seed = 0;
//    mainNSGA(5, 4, seed);
    
//    test_problem(5, 5, 2, 5, 5);
    
//    cout << Utility::millis();
    
//    int problemSize = 10;
//    FitnessFunction * fit = new ZeroMaxOneMax(problemSize);
//    FitnessFunction * fit = new TrapInverseTrap(5, problemSize, 2);
//    GA* ga = new NSGA_II(fit, new TwoPointCrossover(), 2, 0.9, true, false);
//    GA* ga = new MO_LS(fit, Utility::Order::ASCENDING);
    
//    ga->setPopulationSize(10);
//    ga->initialize();
//    ga->evaluateAll();
//    ga->run();

//    int popsize = ga->findMinimallyNeededPopulationSize(100, 100, 1024);
//    cout << popsize << endl;

//    ARK * ark = new ARK7(0, false, true);
//    ark->doAnalysis(6, 13);
    
    return 0;
}


