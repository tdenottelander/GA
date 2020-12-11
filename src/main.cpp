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
#include "ARK8.hpp"
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
#include "MO_GOMEA.hpp"
#include "ProblemType.hpp"
#include "LearnedLTFOS.hpp"
#include <stdlib.h>

#include "LocalSearchAnalysis.hpp"

// Include for the original NSGA-II implementation.
//#include "global.h"

using namespace std;
using namespace Utility;
using nlohmann::json;

// The random generator used throughout the whole application
long mySeed = millis();
mt19937 rng(mySeed);
// A distribution that is used to get values between 0.0 and 1.0 by use of the rng defined above
uniform_real_distribution<float> dist(0.0, 0.9999);

// MAC OS
string projectDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/";
// Ross@CWI
//string projectDir = "/export/scratch1/tdo/TomGA/";

string dataDir = projectDir + "data/";
string benchmarksDir = projectDir + "benchmarks/";
string queuelistDir = "/export/scratch1/home/shared/GA/queue/";
string writeDir;
string path_JSON_MO_info;
string path_JSON_SO_info;
string path_JSON_LS_Results;
string path_JSON_Progress;
string path_JSON_Run;
string path_JSON_FOSElementSuccess;
string path_JSON_ForcedImprovement;
string path_JSON_MIM;

std::vector<std::unordered_map<std::string, std::vector<int>>> FOSElementSuccessPerGeneration_accuracy;
std::vector<std::unordered_map<std::string, std::vector<int>>> FOSElementSuccessPerGeneration_mmacs;
std::vector<std::unordered_map<std::string, std::vector<int>>> FOSElementSuccessPerGeneration_mixed;
std::vector<int> FOSLearnCount_accuracy (10000, 0);
std::vector<int> FOSLearnCount_mmacs (10000, 0);
std::vector<int> FOSLearnCount_mixed (10000, 0);
std::vector<std::vector<int>> ForcedImprovementUsedPerGeneration; //First idx = generation. Vector = {[used, notused]}.
int currentGeneration;
int currentPopulation;
int currentObjective;
bool isExtremeCluster;

string dataset = "cifar100";

json JSON_MO_info;
json JSON_SO_info;
json JSON_LS_Results;
json JSON_Progress;

bool printfos = false;
bool printPopulationAfterRound = false;
bool printPopulationOnOptimum = false;
bool printElitistArchiveOnUpdate = false;
bool printProgressOnIntervals = false;
bool storeConvergence = false;
bool storeAbsoluteConvergence = false;
bool storeUniqueConvergence = true;
bool storeTransformedUniqueConvergence = false;
bool storeDistanceToParetoFrontOnElitistArchiveUpdate = true;
bool storeElitistArchive = true;
bool updateElitistArchiveOnEveryEvaluation = true;
int loggingIntervalMode = 0; // 0 = on a log10 scale, 1 = linear scale
int loggingLinearInterval = 10;
std::string ARK_Analysis_suffix = "";
int populationInitializationMode = 1; // 0 = True Random, 1 = ARK (first all identity individual), 2 = Solvable
bool saveLogFilesOnEveryUpdate = false;
bool saveLSArchiveResults = false;
bool customOutputFolder = false;

// Termination criteria
int maxRounds = -1;
int maxSeconds = -1;
int maxPopSizeLevel = 50;
int maxEvaluations = INT_MAX;
int maxUniqueEvaluations = INT_MAX;
int maxNetworkUniqueEvaluations = INT_MAX;

// (non-)IMS parameters
int IMS_Interval = 4;
bool IMS = false;
int nonIMSPopsize = 100;

int numClusters = -1; // -1 = adaptive number of clusters
int extremeClusters = 1; // 0 = false, 1 = true
int startingAmountOfClusters = 3; // 3 = default, 1 more for every subsequent IMS population

// Problem parameters
int problemSize = 14;
int numberOfObjectives = 2;
bool allowIdentityLayers = true;
bool genotypeChecking = false;
bool forcedImprovement = true;

// Experiment parameters
int repetitions = 30;

// JSON
json JSON_experiment;
json JSON_fitfunc;
json JSON_Run;
json JSON_FOSElementSuccessRate;
json JSON_MIM;
//json JSON_ForcedImprovement;

// Printing parameters
bool printFullElitistArchive = false;
bool printEveryEvaluation = false;
int settingInfoStringLength = 40;

FitnessFunction* fitFunc;
GA* ga;
FOS* fos;
Variation* variation;
bool use_MOGOMEA = false;

string gaID(){
    if (use_MOGOMEA){
        return MO_GOMEA::id();
    } else {
        return ga->id();
    }
}

void setJSONdata(){
    string directoryName = Utility::getDateString();
    JSON_experiment["maxPopSizeLevel"] = maxPopSizeLevel;
    JSON_experiment["maxRounds"] = maxRounds;
    JSON_experiment["maxSeconds"] = maxSeconds;
    JSON_experiment["maxEvaluations"] = maxEvaluations;
    JSON_experiment["maxUniqueEvaluations"] = maxUniqueEvaluations;
    JSON_experiment["maxNetworkUniqueEvaluations"] = maxNetworkUniqueEvaluations;
    JSON_experiment["IMS"] = IMS;
    JSON_experiment["IMS_Interval"] = IMS_Interval;
    JSON_experiment["nonIMSPopsize"] = nonIMSPopsize;
    JSON_experiment["allowIdentityLayers"] = allowIdentityLayers;
    JSON_experiment["genotypeChecking"] = genotypeChecking;
    JSON_experiment["forcedImprovement"] = forcedImprovement;
    JSON_experiment["numClusters"] = numClusters;
    JSON_experiment["extremeClusters"] = extremeClusters;
    JSON_experiment["startingAmountOfClusters"] = startingAmountOfClusters;
    JSON_experiment["repetitions"] = repetitions;
    JSON_experiment["optimizer"] = gaID();
    JSON_experiment["populationInitializationMode"] = populationInitializationMode;
    JSON_experiment["seed"] = mySeed;
    JSON_experiment["saveLogFilesOnEveryUpdate"] = saveLogFilesOnEveryUpdate;
    JSON_experiment["saveLSArchiveResults"] = saveLSArchiveResults;
    if(fos != NULL) JSON_experiment["fos"] = fos->id();
    if(variation != NULL) JSON_experiment["variation"] = variation->id();
    JSON_fitfunc["id"] = fitFunc->id();
    JSON_fitfunc["problemSize"] = problemSize;
    JSON_fitfunc["optimum"] = fitFunc->optimum;
    JSON_fitfunc["alphabet"] = fitFunc->problemType->id();
    JSON_fitfunc["ConvergenceCriteria"] = fitFunc->convergenceCriteria;
    JSON_fitfunc["epsilon"] = fitFunc->epsilon;
    JSON_fitfunc["isMO"] = fitFunc->isMO();
    JSON_fitfunc["numberOfParetoPoints"] = fitFunc->trueParetoFront.size();
    JSON_fitfunc["numberOfObjectives"] = numberOfObjectives;
    if (gaID().find("ARK-7") != string::npos) dataset = "cifar100";
    if (gaID().find("ARK-8") != string::npos) dataset = "cifar10";
    JSON_fitfunc["dataset"] = dataset;
    JSON_experiment["fitnessFunction"] = JSON_fitfunc;
    if (!customOutputFolder){
        writeDir = dataDir + directoryName + "_" + fitFunc->id() + "_" + gaID();
        if(IMS){
            writeDir += "_IMS";
        } else if (use_MOGOMEA || (!ga->isRandomSearchAlgorithm && !ga->isLocalSearchAlgorithm)){
            writeDir += ("_pop=" + to_string(nonIMSPopsize));
        }
    }
    mkdir(writeDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    string filename = writeDir + "/experiment.json";
    writeRawData(JSON_experiment.dump(), filename);
    
    cout << "Creating directory " << writeDir << " for writing results to." << endl;
}

void setDirectories(){
    dataDir = projectDir + "data/";
    cout << "Setting data directory to " << dataDir << endl;
    benchmarksDir = projectDir + "benchmarks/";
    cout << "Setting benchmark directory to " << benchmarksDir << endl;
}

void setFitnessFunction(const char * argv[], int i){
    try {
        problemSize = stoi(argv[i+1]);
    } catch (exception) {}
    try {
        numberOfObjectives = stoi(argv[i+2]);
    } catch (exception) {}

    if (strcmp(argv[i], "zmom") == 0){
        fitFunc = new ZeroMaxOneMax(problemSize);
    } else if (strcmp(argv[i], "lotz") == 0){
        fitFunc = new LOTZ(problemSize);
    } else if (strcmp(argv[i], "tit") == 0){
        fitFunc = new TrapInverseTrap(problemSize);
    } else if (strcmp(argv[i], "maxcut") == 0){
        fitFunc = new MAXCUT(problemSize);
    } else if (strcmp(argv[i], "ark1") == 0){
        fitFunc = new ARK1(problemSize, allowIdentityLayers);
    } else if (strcmp(argv[i], "ark2") == 0){
        fitFunc = new ARK2(problemSize, allowIdentityLayers, genotypeChecking);
    } else if (strcmp(argv[i], "ark3") == 0){
        fitFunc = new ARK3();
    } else if (strcmp(argv[i], "ark4") == 0){
        fitFunc = new ARK4(problemSize, allowIdentityLayers);
    } else if (strcmp(argv[i], "ark5") == 0){
        fitFunc = new ARK5(problemSize, allowIdentityLayers);
    } else if (strcmp(argv[i], "ark6") == 0){
        fitFunc = new ARK6(problemSize, genotypeChecking);
    } else if (strcmp(argv[i], "ark7") == 0){
        fitFunc = new ARK7(problemSize, genotypeChecking, numberOfObjectives == 2);
    } else if (strcmp(argv[i], "ark8") == 0){
        fitFunc = new ARK8(problemSize, genotypeChecking, numberOfObjectives == 2);
    } else if (strcmp(argv[i], "ark-online") == 0){
//        cout << "Add python to run ark-online. Exiting now." << endl;
//        exit(0);
        fitFunc = new ARK_Online(problemSize, numberOfObjectives);
    } else if (strcmp(argv[i], "onemax") == 0){
        fitFunc = new OneMax(problemSize);
    } else if (strcmp(argv[i], "leadingones") == 0){
        fitFunc = new LeadingOnes(problemSize);
    } else if (strcmp(argv[i], "trap") == 0){
        // Might not work yet
        fitFunc = new Trap(5, 5);
    } else if (strcmp(argv[i], "NK") == 0){
        // Might not work yet
        fitFunc = new NK(problemSize, 3, false, 3);
    }
    if(fitFunc == NULL){
        cout << "Could not read fitfunc argument '" << argv[i] << "'. Use -? to see info on arguments. Exiting now." << endl;
        exit(0);
    }
    fitFunc->maxEvaluations = maxEvaluations;
    fitFunc->maxUniqueEvaluations = maxUniqueEvaluations;
    fitFunc->maxNetworkUniqueEvaluations = maxNetworkUniqueEvaluations;
    cout << Utility::padWithSpacesAfter("Setting fitfunc to ", settingInfoStringLength) << fitFunc->id() << endl;
    cout << Utility::padWithSpacesAfter("Setting problemsize to ", settingInfoStringLength) << problemSize << endl;
    cout << Utility::padWithSpacesAfter("Setting numberOfObjectives to ", settingInfoStringLength) << numberOfObjectives << endl;
}

Utility::Order getOrder(const char * orderString){
    Utility::Order order = Utility::Order::RANDOM; // Default random
    if (strcmp(orderString, "rand") == 0){
        order = Utility::Order::RANDOM;
    } else if (strcmp(orderString, "asc") == 0){
        order = Utility::Order::ASCENDING;
    } else if (strcmp(orderString, "desc") == 0){
        order = Utility::Order::DESCENDING;
    }
    return order;
}

void setOptimizer(const char * argv[], int i){

    const char * orderString = argv[i+1];
    Utility::Order order = getOrder(orderString);

    if (strcmp(argv[i], "NSGA-II") == 0){
//        mkdir(queuelistDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (variation == NULL){
            variation = new TwoPointCrossover();
        }
        ga = new NSGA_II(fitFunc, variation, 0.9f, (1.0f / fitFunc->totalProblemLength));
    } else if (strcmp(argv[i], "MO-RS") == 0){
        ga = new MO_RS(fitFunc);
    } else if (strcmp(argv[i], "RSUSS") == 0){
        ga = new MO_RS(fitFunc, true);
    } else if (strcmp(argv[i], "MO-LS") == 0){
        ga = new MO_LS(fitFunc);
    } else if (strcmp(argv[i], "MO-GOMEA") == 0){
        for (int i = 0; i < 1000; i++){
//            unordered_map<string, vector<int>> map;
            FOSElementSuccessPerGeneration_accuracy.push_back(unordered_map<string, vector<int>>());
            FOSElementSuccessPerGeneration_mmacs.push_back(unordered_map<string, vector<int>>());
            FOSElementSuccessPerGeneration_mixed.push_back(unordered_map<string, vector<int>>());
            ForcedImprovementUsedPerGeneration.push_back({0,0});
        }
        use_MOGOMEA = true;
    } else if (strcmp(argv[i], "GOM") == 0){
        ga = new GOM(fitFunc, fos, forcedImprovement);
    } else if (strcmp(argv[i], "GOM-LS") == 0){
        ga = new GOM_LS(fitFunc, fos, new LocalSearch(fitFunc, order), forcedImprovement);
    } else if (strcmp(argv[i], "RS") == 0){
        ga = new RandomSearch(fitFunc);
    } else if (strcmp(argv[i], "SimpleGA") == 0){
        ga = new SimpleGA(fitFunc, variation, new TournamentSelection(2));
    } else if (strcmp(argv[i], "LS") == 0){
        ga = new LocalSearch(fitFunc, order);
    } else if (strcmp(argv[i], "LSS-0.01") == 0){
        ga = new LocalSearchStochastic(fitFunc, order, 0.01);
    } else if (strcmp(argv[i], "LSS-0.05") == 0){
        ga = new LocalSearchStochastic(fitFunc, order, 0.05);
    }
    if(!use_MOGOMEA && ga == NULL){
        cout << "Could not read optimizer argument '" << argv[i] << "'. Use -? to see info on arguments. Exiting now." << endl;
        exit(0);
    }
    cout << Utility::padWithSpacesAfter("Setting optimizer to ", settingInfoStringLength) << gaID() << endl;
}

void setFOS(const char * argv[], int i){

    const char * orderString = argv[i+1];
    Utility::Order order = getOrder(orderString);

    if (strcmp(argv[i], "learned") == 0){
        fos = new LearnedLT_FOS(fitFunc->problemType);
    } else if (strcmp(argv[i], "uni") == 0){
        fos = new Univariate_FOS(order);
    } else if (strcmp(argv[i], "IncrLT") == 0){
        fos = new IncrementalLT_FOS();
    } else if (strcmp(argv[i], "IncrLTR") == 0){
        fos = new IncrementalLTReversed_FOS();
    } else if (strcmp(argv[i], "IncrLT_uni") == 0){
        fos = new IncrementalLT_UnivariateOrdered_FOS();
    } else if (strcmp(argv[i], "IncrLTR_uni") == 0){
        fos = new IncrementalLTReversed_Univariate_FOS();
    } else if (strcmp(argv[i], "IncrLTR_uniOrd") == 0){
        fos = new IncrementalLTReversed_UnivariateOrdered_FOS();
    } else if (strcmp(argv[i], "triplet") == 0){
        fos = new Triplet_FOS(order);
    } else if (strcmp(argv[i], "tripletTree") == 0){
        fos = new TripletTree_FOS(order);
    } else if (strcmp(argv[i], "ark6") == 0){
        fos = new ARK6_FOS(order);
    } else if (strcmp(argv[i], "RT") == 0){
        fos = new RandomTree_FOS();
    } else if (strcmp(argv[i], "RandLT") == 0){
        fos = new RandomLT_FOS(fitFunc->problemType);
    } else if (strcmp(argv[i], "TupleTree") == 0){
        fos = new TupleTree_FOS(order);
    }
    if(fos == NULL){
        cout << "Could not read fos argument '" << argv[i] << "'. Use -? to see info on arguments. Exiting now." << endl;
        exit(0);
    }
    cout << Utility::padWithSpacesAfter("Setting FOS to ", settingInfoStringLength) << fos->id() << endl;
}

void setVariation(const char * argv[], int i){
    if (strcmp(argv[i], "1p") == 0){
        variation = new OnePointCrossover();
    } else if (strcmp(argv[i], "2p") == 0){
        variation = new TwoPointCrossover();
    } else if (strcmp(argv[i], "3p") == 0){
        variation = new ThreePointCrossover();
    } else if (strcmp(argv[i], "uni") == 0){
        variation = new UnivariateCrossover();
    } else if (strcmp(argv[i], "ark6") == 0){
        variation = new ARK6_Crossover();
    }
    if(variation == NULL){
        cout << "Could not read variation argument '" << argv[i] << "'. Use -? to see info on arguments. Exiting now." << endl;
        exit(0);
    }
    cout << Utility::padWithSpacesAfter("Setting variation to ", settingInfoStringLength) << variation->id() << endl;
}

void printCommandLineHelp(){
    cout << "-?: print help" << endl;
    cout << "-P [#1]: set project directory to #1  !!IMPORTANT THAT THIS IS DONE!!" << endl;
    cout << "-S [#1]: set seed to #1 (any integer number)" << endl;
    cout << "-e [#1]: set max evaluations to #1" << endl;
    cout << "-u [#1]: set max unique evaluations to #1" << endl;
    cout << "-n [#1]: set max network unique evaluations to #1" << endl;
    cout << "-m [#1]: set max rounds to #1" << endl;
    cout << "-s [#1]: set max seconds to #1" << endl;
    cout << "-d [#1]: set dataset to #1={cifar10, cifar100} for ark-online" << endl;
    cout << "-f [#1][#2][#3]: set fitness function to #1={zmom, lotz, tit, maxcut, ark1, ark2, ark3, ark4, ark5, ark6, ark7, ark-online, onemax, leadingones, trap, NK} with problemsize #2 and number of objectives #3" << endl;
    cout << "-c [#1]: set convergence criteria to #1={entire_pareto, epsilon_pareto, optimal_fitness, epsilon_fitness}" << endl;
    cout << "-E [#1]: set epsilon to #1" << endl;
    cout << "-F [#1][#2]: set FOS to #1={learned, uni, IncrLT, IncrLTR, IncrLT_uni, IncrLTR_uni, IncrLTR_uniOrd, triplet, tripletTree, ark6, RT} with optional order #2={rand, asc, desc}" << endl;
    cout << "-C [#1]: set number of clusters to #1 (default -1 = adaptive. 0 is not valid)" << endl;
    cout << "-X [#1]: set extreme clusters to #1 (default 1 = true, 0 = false)" << endl;
    cout << "-A [#1]: set starting amount of clusters for IMS configuration to #1 (default num of objectives + 1)" << endl;
    cout << "-v [#1]: set variation operator to #1={1p, 2p, 3p, uni, ark6}" << endl;
    cout << "-o [#1][#2]: set optimizer to #1={NSGA-II, MO-RS, MO-LS {loop, noloop} {0 (objectivespace), 1 (random), 2 (scalarizationspace)}, MO-GOMEA, GOM, GOM-LS, RS, SimpleGA, LS, LSS-0.01, LSS-0.05} with optional order #2={rand, asc, desc}" << endl;
    cout << "-r [#1]: set repetitions to #1" << endl;
    cout << "-I [#1]: set IMS to #1={0,1}" << endl;
    cout << "-p [#1]: set non-IMS Popsize to #1" << endl;
    cout << "-i [#1]: set forced improvement to #1={0,1}" << endl;
    cout << "-g [#1]: set genotype checking to #1={0,1}" << endl;
    cout << "-l [#1]: set allow identity layers to #1={0,1}" << endl;
    cout << "-M [#1]: set population initialization mode to #1={0 (true random), 1 (random, but first individual to all identity), 2 (solvable)}" << endl;
    cout << "-a [#1]: set print full elitist archive to #1={0, 1}" << endl;
    cout << "-q [#1]: set print every evaluation to #1={0, 1}" << endl;
    cout << "-x [#1]: set saving log files on every update to #1={0, 1}" << endl;
    cout << "-L [#1]: set saving LS archive results to #1={0, 1}" << endl;
    cout << "-O [#1]: set custom write #1" << endl;
}

void setConvergenceCriteria(const char * argv[], int i){
    if (strcmp(argv[i], "entire_pareto") == 0){
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::ENTIRE_PARETO;
    } else if (strcmp(argv[i], "epsilon_pareto") == 0){
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::EPSILON_PARETO_DISTANCE;
    } else if (strcmp(argv[i], "optimal_fitness") == 0){
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::OPTIMAL_FITNESS;
    } else if (strcmp(argv[i], "epsilon_fitness") == 0){
        fitFunc->convergenceCriteria = FitnessFunction::ConvergenceCriteria::EPSILON_FITNESS;
    }
}

void setParameter(char ch, const char * argv[], int i){
    switch (ch) {
        case '?': printCommandLineHelp(); exit(0); break;
        case 'P':
            projectDir = argv[i];
            setDirectories();
            cout << Utility::padWithSpacesAfter("Setting project directory to ", settingInfoStringLength) << projectDir << endl;
            break;
        case 'S':
            mySeed = stol(argv[i]);
            rng = mt19937(mySeed);
            cout << Utility::padWithSpacesAfter("Setting seed to ", settingInfoStringLength) << mySeed << endl;
            break;
        case 'e':
            maxEvaluations = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting maxEvaluations to ", settingInfoStringLength) << maxEvaluations << endl;
            break;
        case 'u':
            maxUniqueEvaluations = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting maxUniqueEvaluations to ", settingInfoStringLength) << maxUniqueEvaluations << endl;
            break;
        case 'n':
            maxNetworkUniqueEvaluations = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting maxNetworkUniqueEvaluations to ", settingInfoStringLength) << maxNetworkUniqueEvaluations << endl;
            break;
        case 'm':
            maxRounds = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting maxRounds to ", settingInfoStringLength) << maxRounds << endl;
            break;
        case 's':
            maxSeconds = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting maxSeconds to ", settingInfoStringLength) << maxSeconds << endl;
            break;
        case 'd':
            dataset = argv[i];
            cout << Utility::padWithSpacesAfter("Setting dataset to ", settingInfoStringLength) << dataset << endl;
            break;
        case 'f': setFitnessFunction(argv, i); break;
        case 'c': setConvergenceCriteria(argv, i); break;
        case 'E':
            fitFunc->epsilon = stof(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting fitfunc epsilon to ", settingInfoStringLength) << fitFunc->epsilon << endl;
            break;
        case 'F': setFOS(argv, i); break;
        case 'C':
            numClusters = stoi(argv[i]);
            if (numClusters == 0 || numClusters < -1){
                cout << "Not valid number of clusters. Choose -1 (adaptive) or a value > 0. Exiting now" << endl;
                exit(0);
            }
            cout << Utility::padWithSpacesAfter("Setting number of clusters to ", settingInfoStringLength) << numClusters << endl;
            break;
        case 'A':
            startingAmountOfClusters = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting starting amount of clusters to ", settingInfoStringLength) << startingAmountOfClusters << endl;
            break;
        case 'X':
            extremeClusters = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting extreme clusters to ", settingInfoStringLength) << (extremeClusters == 1 ? "True" : "False") << endl;
            break;
        case 'v': setVariation(argv, i); break;
        case 'o': setOptimizer(argv, i); break;
        case 'r':
            repetitions = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting repetitions to ", settingInfoStringLength) << repetitions << endl;
            break;
        case 'I':
            IMS = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting IMS to ", settingInfoStringLength) << IMS << endl;
            break;
        case 'p':
            nonIMSPopsize = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting non-IMS popsize to ", settingInfoStringLength) << nonIMSPopsize << endl;
            break;
        case 'i':
            forcedImprovement = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting forced Improvement to ", settingInfoStringLength) << forcedImprovement << endl;
            break;
        case 'g':
            genotypeChecking = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting genotype checking to ", settingInfoStringLength) << genotypeChecking << endl;
            break;
        case 'l':
            allowIdentityLayers = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting allow Identity Layers to ", settingInfoStringLength) << allowIdentityLayers << endl;
            break;
        case 'M':
            populationInitializationMode = stoi(argv[i]);
            cout << Utility::padWithSpacesAfter("Setting pop initialization mode to ", settingInfoStringLength) << populationInitializationMode << endl;
            break;
        case 'a':
            printFullElitistArchive = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting printFullElitistArchive to ", settingInfoStringLength) << printFullElitistArchive << endl;
            break;
        case 'q':
            printEveryEvaluation = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting printEveryEvaluation to ", settingInfoStringLength) << printFullElitistArchive << endl;
            break;
        case 'x':
            saveLogFilesOnEveryUpdate = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting saveLogFilesOnEveryUpdate to ", settingInfoStringLength) << saveLogFilesOnEveryUpdate << endl;
            break;
        case 'L':
            saveLSArchiveResults = stoi(argv[i]) == 1;
            cout << Utility::padWithSpacesAfter("Setting saveLSArchiveResults to ", settingInfoStringLength) << saveLSArchiveResults << endl;
            break;
        case 'O':
            customOutputFolder = true;
            writeDir = dataDir + argv[i];
            cout << Utility::padWithSpacesAfter("Setting custom output write folder to ", settingInfoStringLength) << writeDir << endl;
            break;
    }
}

void parseCommandLineArguments(int argc, const char * argv[]){
    string executable = argv[0];
    string allArguments = "";
    for (int i = 1; i < argc; i++){
        allArguments += argv[i];
        allArguments += " ";
    }
    cout << "Command Line Arguments: " << allArguments << endl;
    JSON_experiment["commandLineExecutable"] = executable;
    JSON_experiment["commandLineArguments"] = allArguments;
    
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            setParameter(argv[i][1], argv, i+1);
        }
    }
    cout << "Project directory set to " << projectDir << endl;
    cout << "Make sure this is correct! ^^^^^ Otherwise, add it as argument with '-P [projectDirectory]'" << endl;
    cout << "Seed = " << mySeed << "\n" << endl;
}

void printRepetition(int rep){
    cout << "rep" << padWithSpacesAfter(to_string(rep), 2)
    << " ga=" << gaID()
    << " l=" << problemSize
    << " success=" << padWithSpacesAfter(to_string(JSON_Run.at("success")), 7)
    << " time=" << padWithSpacesAfter(to_string(JSON_Run.at("time_taken")), 12)
    << " evals=" << padWithSpacesAfter(to_string(fitFunc->totalEvaluations), 15)
    << " uniqEvals=" << padWithSpacesAfter(to_string(fitFunc->totalUniqueEvaluations), 15);
    if(fitFunc->storeNetworkUniqueEvaluations) cout << " networkUniqEvals=" << padWithSpacesAfter(to_string(fitFunc->totalNetworkUniqueEvaluations), 15);
    if(storeTransformedUniqueConvergence) cout << " trUniqEvals=" << padWithSpacesAfter(to_string(fitFunc->totalTransformedUniqueEvaluations), 15);
    cout << endl;
}

void writeFOSElementSuccessToJSON (vector<unordered_map<string, vector<int>>> &FOSElementSuccessPerGeneration, vector<int> &FOSLearnCount, string keyValue){
    for (int i = 0; i < FOSElementSuccessPerGeneration.size(); i++){
        json generationalFOSElementSuccess;
        for (auto it = FOSElementSuccessPerGeneration[i].begin(); it != FOSElementSuccessPerGeneration[i].end(); ++it ){
            string key = it->first;
            vector<int> value = it->second;
            generationalFOSElementSuccess[key] = value;
        }
        JSON_FOSElementSuccessRate[keyValue][to_string(i)] = generationalFOSElementSuccess;
        JSON_FOSElementSuccessRate["FOSLearnCount"][keyValue] = FOSLearnCount;
    }
}

void performExperiment(){
    vector<int> evals;
    vector<int> uniqueEvals;
    vector<int> networkUniqueEvals;
    vector<int> times;
    path_JSON_FOSElementSuccess = writeDir + "/FOSElementSuccess.json";
    path_JSON_ForcedImprovement = writeDir + "/ForcedImprovement.json";

    for (int rep = 0; rep < repetitions; rep++){
        path_JSON_Progress = writeDir + "/progress" + to_string(rep) + ".json";
        path_JSON_MO_info = writeDir + "/MO_info" + to_string(rep) + ".json";
        path_JSON_SO_info = writeDir + "/SO_info" + to_string(rep) + ".json";
        path_JSON_LS_Results = writeDir + "/LS_results" + to_string(rep) + ".json";
        path_JSON_Run = writeDir + "/run" + to_string(rep) + ".json";
        path_JSON_MIM = writeDir + "/MIM" + to_string(rep) + ".json";

        JSON_Progress.clear();
        JSON_MO_info.clear();
        JSON_SO_info.clear();
        JSON_LS_Results.clear();
        JSON_Run.clear();
        JSON_MIM.clear();
        fitFunc->clear();

        if(use_MOGOMEA){
            MO_GOMEA().main_MO_GOMEA();
        } else {
            RoundSchedule rs (maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, maxUniqueEvaluations, maxNetworkUniqueEvaluations, IMS_Interval);

            rs.initialize(ga, problemSize, IMS, nonIMSPopsize);

            rs.run();
        }
        printRepetition(rep);

        times.push_back(JSON_Run.at("time_taken"));
        evals.push_back(fitFunc->totalEvaluations);
        uniqueEvals.push_back(fitFunc->totalUniqueEvaluations);
        networkUniqueEvals.push_back(fitFunc->totalNetworkUniqueEvaluations);

        writeRawData(JSON_Run.dump(), path_JSON_Run);
        if (numberOfObjectives > 1) writeRawData(JSON_MO_info.dump(), path_JSON_MO_info);
        else writeRawData(JSON_SO_info.dump(), path_JSON_SO_info);
        if (saveLSArchiveResults && gaID().find("MO-LS") != string::npos) writeRawData(JSON_LS_Results, path_JSON_LS_Results);
//        writeRawData(JSON_MIM.dump(), path_JSON_MIM);
    }
    cout << endl;

    cout << "Avg Time: " << Utility::getAverage(times) << endl;
    cout << "Avg Evals: " << Utility::getAverage(evals) << endl;
    cout << "Avg Unique Evals: " << Utility::getAverage(uniqueEvals) << endl;
    cout << "Avg Network Unique Evals: " << Utility::getAverage(networkUniqueEvals) << endl;
    if (numberOfObjectives > 1) fitFunc->printElitistArchive(printFullElitistArchive);

    if(use_MOGOMEA){
        
        writeFOSElementSuccessToJSON(FOSElementSuccessPerGeneration_accuracy, FOSLearnCount_accuracy, "accuracy");
        writeFOSElementSuccessToJSON(FOSElementSuccessPerGeneration_mmacs, FOSLearnCount_mmacs, "mmacs");
        writeFOSElementSuccessToJSON(FOSElementSuccessPerGeneration_mixed, FOSLearnCount_mixed, "mixed");
        writeRawData(JSON_FOSElementSuccessRate.dump(), path_JSON_FOSElementSuccess);
        json JSON_ForcedImprovement (ForcedImprovementUsedPerGeneration);
//        for (int i = 0; i < 5; i++){
//            JSON_ForcedImprovement[i] = ForcedImprovementUsedPerGeneration[i];
//        }
        writeRawData(JSON_ForcedImprovement.dump(), path_JSON_ForcedImprovement);
    }
}

void run(int argc, const char * argv[]){
    parseCommandLineArguments(argc, argv);
    setJSONdata();
    performExperiment();
}

void bisection(){
    int neededPopsize = ga->findMinimallyNeededPopulationSize(100, 99);
    cout << "Needed popsize = " << neededPopsize << endl;
}

int main(int argc, const char * argv[]) {

//    char mypath[]="PYTHONHOME=/Users/tomdenottelander/miniconda3/envs/nasbench/";
//    putenv( mypath );
    
//    ARK8(1, false, true).doAnalysis(1, 14);

    run(argc, argv);

    return 0;
}
