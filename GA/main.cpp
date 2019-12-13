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
#include <Python.h>
#include "Individual.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "FitnessFunction.hpp"
#include "Trap.hpp"
#include "NASBench.hpp"
#include "Utility.hpp"
#include "RoundSchedule.hpp"
#include "GA.hpp"
#include "GOM.hpp"
#include "SimpleGA.hpp"
//#include "Stuff.hpp"
#include "ProblemType.hpp"
#include "LearnedLTFOS.hpp"
#include <stdlib.h>

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

extern bool printfos = false;
extern bool printPopulationAfterRound = false;
extern bool printPopulationOnOptimum = false;
extern PyObject *module = NULL;

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
    
    FitnessFunction * fit = new Trap(5);
    main_json["fitnessFunction"] = fit->id();
    
    vector<GA*> gaList = {
        new GOM(fit, new LearnedLT_FOS(fit->problemType), true),
        new SimpleGA(fit, new UnivariateCrossover(), new TournamentSelection(2)),
        new SimpleGA(fit, new OnePointCrossover(), new TournamentSelection(2)),
        new GOM(fit, new Univariate_FOS(), true),
        new GOM(fit, new UnivariateOrdered_FOS(), true),
        new GOM(fit, new IncrementalLT_FOS(), true),
        new GOM(fit, new IncrementalLT_Univariate_FOS(), true)
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
    json main_json;
    
    // Set to -1 if it should not be a stopping condition
    int maxRounds = -1;
    int maxSeconds = -1;
    int maxPopSizeLevel = 100;
    int maxEvaluations = 1000000;
    int interval = 4;
    int repetitions = 10;
    
    main_json["maxPopSizeLevel"] = maxPopSizeLevel;
    main_json["maxRounds"] = maxRounds;
    main_json["maxSeconds"] = maxSeconds;
    main_json["maxEvaluations"] = maxEvaluations;
    main_json["repetitions"] = repetitions;
    main_json["interleavedRoundInterval"] = interval;
    
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Append(sysPath, PyUnicode_FromString("/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/nasbench/"));
    //    PyObject *module = PyImport_ImportModule("queryscript");
    module = PyImport_ImportModule("queryscript");
    if(module == NULL){
        printf("ERROR importing module 'queryscript'\n");
        exit(-1);
    }
    PyObject* py_loaddatafunc = PyObject_GetAttrString(module, "load_data");
    if(!py_loaddatafunc){
        PyErr_Print();
        exit(-1);
    }
    PyObject_CallObject(py_loaddatafunc, NULL);
    
    PyObject* py_queryfunc = PyObject_GetAttrString(module, "query");
    
    FitnessFunction * fit = new NASBench(NULL, py_queryfunc);
    main_json["fitnessFunction"] = fit->id();
    
    vector<GA*> gaList = {
        new GOM(fit, new LearnedLT_FOS(fit->problemType), true),
        new SimpleGA(fit, new UnivariateCrossover(), new TournamentSelection(2)),
        new SimpleGA(fit, new OnePointCrossover(), new TournamentSelection(2)),
        new GOM(fit, new Univariate_FOS(), true),
        new GOM(fit, new UnivariateOrdered_FOS(), true),
        new GOM(fit, new IncrementalLT_FOS(), true),
        new GOM(fit, new IncrementalLT_Univariate_FOS(), true)
    };
    
    
    json experiments;
    int problemSize = 5;
    for(GA* ga : gaList){
        json setting;
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
                break;
            } else if(result.at("stoppingCondition") == "maxEvaluationsExceeded"){
                cout << "Max evaluations exceeded, not starting anymore runs" << endl;
                break;
            }
        }
        cout << endl;
        setting[to_string(problemSize)] = prob_json;
        experiments[ga->id()] = setting;
    }
    
    main_json["experiments"] = experiments;
    write(main_json.dump(), dataDir);
    
    Py_Finalize();
}

void pythonLink(){
    Py_Initialize();
    
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Append(sysPath, PyUnicode_FromString("/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/nasbench/"));
    PyObject *module = PyImport_ImportModule("query");
    if(module == NULL){
        printf("ERROR importing module 'query'\n");
        exit(-1);
    }
    
    // Loading the data
    PyObject* py_loaddatafunc = PyObject_GetAttrString(module, "load_data");
    if(!py_loaddatafunc){
        PyErr_Print();
        exit(-1);
    }
    PyObject_CallObject(py_loaddatafunc, NULL);
    
//    // Setting a model
//    PyObject* py_setmodelfunc = PyObject_GetAttrString(module, "set_model");
//    if(!py_setmodelfunc){
//        PyErr_Print();
//        exit(-1);
//    }
//    PyObject* py_args = Py_BuildValue("[iiiii]", 0, 1, 0, 0, 0);
//    PyObject_CallObject(py_setmodelfunc, py_args);
//
//
//    // Printing the model
//    PyObject* py_printmodelfunc = PyObject_GetAttrString(module, "print_model");
//    if(!py_printmodelfunc){
//        PyErr_Print();
//        exit(-1);
//    }
//    PyObject_CallObject(py_printmodelfunc, NULL);
    
    
    // Querying the model
    PyObject* py_querymodelfunc = PyObject_GetAttrString(module, "query");
    if(!py_querymodelfunc){
        PyErr_Print();
        exit(-1);
    }
    PyObject* py_args = Py_BuildValue("[iiiii]", 0, 1, 0, 0, 0);
    PyObject_CallObject(py_querymodelfunc, py_args);
    
//    load_data()
//    set_model([0, 1, 2, 1, 2])
//    print_model()
//    query()
    
    Py_Finalize();
}


int main(int argc, const char * argv[]) {
    
    char mypath[]="PYTHONHOME=/Users/tomdenottelander/miniconda3/envs/nasbench/";
    putenv( mypath );
//    std::cout << "PYTHONHOME = " << getenv("PYTHONHOME") << std::endl;
    
    runNasbench();
//    roundSchedule();
    
    return 0;
}


