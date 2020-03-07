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
#include <sys/stat.h>
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

// MAC OS
string projectDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/";
// Ross@CWI
//string projectDir = "/export/scratch1/tdo/TomGA/";

string dataDir = projectDir + "data/";
string benchmarksDir = projectDir + "benchmarks/";
string writeDir;

extern json JSON_MO_info;

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
int populationInitializationMode = 1; // 0 = True Random, 1 = ARK (first all identity individual), 2 = Solvable

// Termination criteria
int maxRounds = -1;
int maxSeconds = -1;
int maxPopSizeLevel = 500;
int maxEvaluations = 9999999;
int maxUniqueEvaluations = 9999999;

// (non-)IMS parameters
int IMS_Interval = 4;
bool IMS = false;
int nonIMSPopsize = 40;

// Problem parameters
int problemSize = 14;
bool allowIdentityLayers = true;
bool genotypeChecking = false;
bool forcedImprovement = true;

// Experiment parameters
int repetitions = 30;

// JSON
json JSON_experiment;
json JSON_fitfunc;
json JSON_run;
json JSON_MO_information;

FitnessFunction* fitFunc;
GA* ga;
FOS* fos;
bool use_MOGOMEA = false;

void setJSONdata(){
    string directoryName = Utility::getDateString();
    JSON_experiment["maxPopSizeLevel"] = maxPopSizeLevel;
    JSON_experiment["maxRounds"] = maxRounds;
    JSON_experiment["maxSeconds"] = maxSeconds;
    JSON_experiment["maxEvaluations"] = maxEvaluations;
    JSON_experiment["maxUniqueEvaluations"] = maxUniqueEvaluations;
    JSON_experiment["IMS"] = IMS;
    JSON_experiment["IMS_Interval"] = IMS_Interval;
    JSON_experiment["nonIMSPopsize"] = nonIMSPopsize;
    JSON_experiment["allowIdentityLayers"] = allowIdentityLayers;
    JSON_experiment["genotypeChecking"] = genotypeChecking;
    JSON_experiment["forcedImprovement"] = forcedImprovement;
    JSON_experiment["optimizer"] = (use_MOGOMEA ? "MO_GOMEA" : ga->id());
    JSON_fitfunc["id"] = fitFunc->id();
    JSON_fitfunc["problemSize"] = problemSize;
    JSON_fitfunc["optimum"] = fitFunc->optimum;
    JSON_fitfunc["alphabet"] = fitFunc->problemType->id();
    JSON_fitfunc["ConvergenceCriteria"] = fitFunc->convergenceCriteria;
    JSON_fitfunc["epsilon"] = fitFunc->epsilon;
    JSON_fitfunc["isMO"] = fitFunc->isMO();
    JSON_fitfunc["numberOfParetoPoints"] = fitFunc->trueParetoFront.size();
    JSON_experiment["fitnessFunction"] = JSON_fitfunc;
    writeDir = dataDir + directoryName + "_" + fitFunc->id() + "_" + (use_MOGOMEA ? "MO_GOMEA" : ga->id());
    if(mkdir(writeDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0){
        string filename = writeDir + "/experiment.json";
        writeRawData(JSON_experiment.dump(), filename);
    }
}


void setFitnessFunction(const char * argv[], int i){
//    cout << argv[i] << argv[i+1] << endl;
    problemSize = stoi(argv[i+1]);
    if (strcmp(argv[i], "zmom") == 0){
        cout << "ZMOM" << endl;
        fitFunc = new ZeroMaxOneMax(problemSize);
    } else if (strcmp(argv[i], "lotz") == 0){
        cout << "lotz" << endl;
        fitFunc = new LOTZ(problemSize);
    } else if (strcmp(argv[i], "tit") == 0){
        cout << "tit" << endl;
        fitFunc = new TrapInverseTrap(problemSize);
    } else if (strcmp(argv[i], "maxcut") == 0){
        cout << "maxcut" << endl;
        fitFunc = new MAXCUT(problemSize);
    } else if (strcmp(argv[i], "ark7") == 0){
        cout << "ark7" << endl;
        fitFunc = new ARK7(problemSize, genotypeChecking, true);
    }
    fitFunc->maxEvaluations = maxEvaluations;
    fitFunc->maxUniqueEvaluations = maxUniqueEvaluations;
}

void setOptimizer(const char * argv[], int i){
    
    //Currently only support for MO algorithms
    if (strcmp(argv[i], "NSGA-II") == 0){
        ga = new NSGA_II(fitFunc, new TwoPointCrossover(), 0.9, true);
    } else if (strcmp(argv[i], "MO-RS") == 0){
        ga = new MO_RS(fitFunc);
    } else if (strcmp(argv[i], "MO-LS") == 0){
        ga = new MO_LS(fitFunc, Utility::Order::RANDOM, 100000);
    } else if (strcmp(argv[i], "MO-GOMEA") == 0){
        use_MOGOMEA = true;
    }
}

void setFOS(const char * argv[], int i){
    if (strcmp(argv[i], "learned") == 0){
        fos = new LearnedLT_FOS(fitFunc->problemType);
    } else if (strcmp(argv[i], "uni") == 0){
        fos = new Univariate_FOS(Utility::Order::RANDOM);
    }
}

void printCommandLineHelp(){
    cout << "-?: print help" << endl;
    cout << "-e [#1]: set max evaluations to #1" << endl;
    cout << "-u [#1]: set max unique evaluations to #1" << endl;
    cout << "-m [#1]: set max rounds to #1" << endl;
    cout << "-s [#1]: set max seconds to #1" << endl;
    cout << "-f [#1][#2]: set fitness function to #1={zmom, lotz, tit, maxcut, ark7} with problemsize #2" << endl;
    cout << "-c [#1]: set convergence criteria to #1={entire_pareto, epsilon_pareto}" << endl;
    cout << "-E [#1]: set epsilon to #1" << endl;
    cout << "-F [#1]: set FOS to #1={learned, uni}" << endl;
    cout << "-o [#1]: set optimizer to #1={NSGA-II, MO-RS, MO-LS, MO-GOMEA}" << endl;
    cout << "-r [#1]: set repetitions to #1" << endl;
    cout << "-I [#1]: set IMS to #1={0,1}" << endl;
    cout << "-p [#1]: set non-IMS Popsize to #1" << endl;
}

void setConvergenceCriteria(const char * argv[], int i){
    if (strcmp(argv[i], "entire_pareto") == 0){
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::ENTIRE_PARETO;
    } else if (strcmp(argv[i], "epsilon_pareto") == 0){
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::EPSILON_PARETO_DISTANCE;
    }
}

void setParameter(char ch, const char * argv[], int i){
    switch (ch) {
        case '?': printCommandLineHelp(); exit(0); break;
        case 'e': maxEvaluations = stoi(argv[i]); break;
        case 'u': maxUniqueEvaluations = stoi(argv[i]); break;
        case 'm': maxRounds = stoi(argv[i]); break;
        case 's': maxSeconds = stoi(argv[i]); break;
        case 'f': setFitnessFunction(argv, i); break;
        case 'c': setConvergenceCriteria(argv, i); break;
        case 'E': fitFunc->epsilon = stof(argv[i]); break;
        case 'F': setFOS(argv, i); break;
        case 'o': setOptimizer(argv, i); break;
        case 'r': repetitions = stoi(argv[i]); break;
        case 'I': IMS = stoi(argv[i]) == 1; break;
        case 'p': nonIMSPopsize = stoi(argv[i]); break;
    }
}

void parseCommandLineArguments(int argc, const char * argv[]){
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            setParameter(argv[i][1], argv, i+1);
        }
    }
}

void printRepetition(int rep){
    cout << "rep" << padWithSpacesAfter(to_string(rep), 2)
    << " ga=" << (use_MOGOMEA ? "MO-GOMEA" : ga->id())
    << " l=" << problemSize
    << " success=" << padWithSpacesAfter(to_string(JSON_run.at("success")), 7)
    << " time=" << padWithSpacesAfter(to_string(JSON_run.at("time_taken")), 12)
    << " evals=" << padWithSpacesAfter(to_string(fitFunc->totalEvaluations), 15)
    << " uniqEvals=" << padWithSpacesAfter(to_string(fitFunc->totalUniqueEvaluations), 15);
    if(storeTransformedUniqueConvergence) cout << " trUniqEvals=" << padWithSpacesAfter(to_string(fitFunc->totalTransformedUniqueEvaluations), 15);
    cout << endl;
}

void performExperiment(){
    string outputfileName = dataDir + Utility::getDateString() + "_rawdata.json";
    vector<int> evals;
    vector<int> uniqueEvals;
    vector<int> times;
    
    for (int rep = 0; rep < repetitions; rep++){
        JSON_run.clear();
        JSON_MO_info.clear();
        fitFunc->clear();
        long time;
        
        if(use_MOGOMEA){
            MO_GOMEA().main_MO_GOMEA();
        } else {
            RoundSchedule rs (maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, maxUniqueEvaluations, IMS_Interval);
            
            rs.initialize(ga, problemSize, IMS, nonIMSPopsize);
            
            JSON_run = rs.run();
        }
        printRepetition(rep);
        
        times.push_back(time);
        evals.push_back(fitFunc->totalEvaluations);
        uniqueEvals.push_back(fitFunc->totalUniqueEvaluations);
        
        writeRawData(JSON_run.dump(), writeDir + "/run" + to_string(rep) + ".json");
        writeRawData(JSON_MO_info.dump(), writeDir + "/MO_info" + to_string(rep) + ".json");
    }
    cout << endl;
    
    cout << "Avg Time: " << Utility::getAverage(times) << endl;
    cout << "Avg Evals: " << Utility::getAverage(evals) << endl;
    cout << "Avg Unique Evals: " << Utility::getAverage(uniqueEvals) << endl;
    fitFunc->printElitistArchive();
}

void run(int argc, const char * argv[]){
    parseCommandLineArguments(argc, argv);
    setJSONdata();
    performExperiment();
}

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
    
    int minProblemSize = 14;
    int maxProblemSize = 14;
    
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
        
//        FitnessFunction * fit = new Trap(5, 5);
//        FitnessFunction * fit = new OneMax(20);
//        FitnessFunction * fit = new LeadingOnes(20);
        
        FitnessFunction * fit = new ZeroMaxOneMax(problemSize);
//        FitnessFunction * fit = new LOTZ(problemSize);
//        FitnessFunction * fit = new TrapInverseTrap(problemSize);
//        FitnessFunction * fit = new MAXCUT(problemSize);
//        fit->convergenceCriteria = FitnessFunction::ConvergenceCriteria::ENTIRE_PARETO;
        fit->convergenceCriteria = FitnessFunction::ConvergenceCriteria::EPSILON_PARETO_DISTANCE;
        fit->epsilon = 0.00001;
        
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
            

            // ------------- MULTI OBJECTIVE ALGORITHMS -------------
            new NSGA_II(fit, new TwoPointCrossover(), 0.9, true),
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
                writeRawData(setting.dump(), dataDir + Utility::getDateString() + "_" + ga->id() + "_elitistArchive.json");
            }
        }
        
        main_json["experiments"] = experiments;
        writeRawData(main_json.dump(), outputfileName);
    }
}

void run_MO_GOMEA(int argc, const char * argv[]){
    if (argc <= 6){
        MO_GOMEA().main_MO_GOMEA();
        exit(0);
    }
    
    int problemsize = stoi(argv[3]);
    
    string problem = argv[7];
    if (problem == "ark7"){
        fitFunc = new ARK7(problemsize, false, true);
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::EPSILON_PARETO_DISTANCE;
        fitFunc->epsilon = 0.0001;
    } else if (problem == "zmom"){
        fitFunc = new ZeroMaxOneMax(problemsize);
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::ENTIRE_PARETO;
        fitFunc->maxEvaluations = 200;
    } else if (problem == "tit"){
        fitFunc = new TrapInverseTrap(problemsize);
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::ENTIRE_PARETO;
    } else {
        cout << "Problem not known. Exiting now" << endl;
        exit(0);
    }
    
    int repetitions = 10;
    
    json main;
    for (int i = 0; i < repetitions; i++){
        fitFunc->clear();
        MO_GOMEA().main_MO_GOMEA();
        main.push_back(JSON_MO_info);
    }
    // TODO: Add different FOS
    string fos = "fos";
    writeRawData(main.dump(), dataDir + Utility::getDateString() + "_MOGOMEA_" + fos + "_elitistArchive.json");
}


void bisection(){
    int neededPopsize = ga->findMinimallyNeededPopulationSize(100, 99);
    cout << "Needed popsize = " << neededPopsize << endl;
}

int main(int argc, const char * argv[]) {
    
    char mypath[]="PYTHONHOME=/Users/tomdenottelander/miniconda3/envs/nasbench/";
    putenv( mypath );
    
//    if (strcmp(argv[1], "nasbench") == 0){
//        runNasbench();
//    } else {
//        run_MO_GOMEA(argc, argv);
//    }
    
    run(argc, argv);
    
    
    return 0;
}


