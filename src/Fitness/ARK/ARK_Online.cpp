//
//  ARK_Online.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK_Online.hpp"

using namespace std;

extern string path_JSON_Progress;
extern nlohmann::json JSON_Progress;
extern string dataset;

ARK_Online::ARK_Online (int problemSize, int numberOfObjectives) : FitnessFunction(getProblemType()) {
    networkLibrary.type = SolutionLibrary::Type::ARK_ONLINE;
    numObjectives = numberOfObjectives;
    totalProblemLength = problemSize;
    storeNetworkUniqueEvaluations = true;
    pythonInit();
}

vector<float> ARK_Online::evaluate(Individual &ind){
    
    vector<float> fitness;
    bool totalNetworkUniqueEvaluationsUpdate = false;
    
    if (networkLibrary.contains(ind.genotype)){
        fitness = networkLibrary.get(ind.genotype);
    } else {
        fitness = pyEvaluate(ind);
        networkLibrary.put(ind.genotype, fitness);
        totalNetworkUniqueEvaluations++;
        totalNetworkUniqueEvaluationsUpdate = true;
    }
    
    ind.fitness = fitness;
    
    evaluationProcedure(ind);

    if(totalNetworkUniqueEvaluationsUpdate && log(totalNetworkUniqueEvaluations)){
        logNetworkUniqueEvaluations();
    }
    logAndSaveProgress(ind);
    
//    cout << ind.toString() << " evals: " << totalEvaluations << " uniqEvals: " << totalUniqueEvaluations << " netUniqEvals: " << totalNetworkUniqueEvaluations << endl;
//    cout << "Lib size: " << networkLibrary.library.size() << endl;
    
    return fitness;
}

vector<float> ARK_Online::pyEvaluate(Individual &ind){
    // Build a python array from the individual's genotype
    int N = ind.genotype.size();
    PyObject* py_args = PyList_New(N);
    for (int i = 0; i < N; i++){
        PyObject* python_int = Py_BuildValue("i", ind.genotype[i]);
        PyList_SetItem(py_args, i, python_int);
    }

    // Create a tuple from the python array
    PyObject* py_tuple = PyTuple_Pack(1, py_args);

    // Check if the reference to the evaluation function is still correct
    if(!py_evaluationFunction){
        PyErr_Print();
        exit(-1);
    }

    // Evaluate the result
    PyObject* result = PyObject_CallObject(py_evaluationFunction, py_tuple);
    if(!result){
        PyErr_Print();
        exit(-1);
    }

    // Unpack the result
    int mmacs = PyLong_AsLong(PyTuple_GetItem(result,0));
    float val_acc = PyFloat_AsDouble(PyTuple_GetItem(result, 1));
    float test_acc = PyFloat_AsDouble(PyTuple_GetItem(result, 2));

    // Return the normalized fitness values
    if (numObjectives == 2){
        return vector<float> {val_acc * normalization[0], 1.0f - (mmacs * normalization[1])};
    } else {
        return vector<float>{val_acc * normalization[0]};
    }
}

void ARK_Online::display(){
    cout << "ARK_Online fitness function" << endl;
}

string ARK_Online::id(){
    return ("ARK-Online-" + to_string(numObjectives) + "-" + dataset);
}

ProblemType* ARK_Online::getProblemType(){
    vector<int> alphabet = {0,1,2,3,4};
    return new AlphabetProblemType(alphabet);
}

FitnessFunction* ARK_Online::clone() const {
    return new ARK_Online(static_cast<const ARK_Online&>(*this));
}

void ARK_Online::setLength (int length){
    totalProblemLength = length;
}

void ARK_Online::pythonInit(){
    
    // Init python connection
    Py_Initialize();
    
    //Path to folder containing python script
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Append(sysPath, PyUnicode_FromString("/export/scratch1/home/shared/nas_online/")); // Meteor folder structure
    PyList_Append(sysPath, PyUnicode_FromString("/export/scratch1/tdo/nas_online/")); // Ross folder structure

    // Import python module/script
    //module = PyImport_ImportModule("ark_online");
    module = PyImport_ImportModule("nas_test");
    if(module == NULL){
        PyErr_Print();
        printf("ERROR importing module 'nas_test'\n");
        exit(-1);
    }

    // Load the data
    PyObject* py_initFunc = PyObject_GetAttrString(module, "init");
    if(!py_initFunc){
        PyErr_Print();
        exit(-1);
    }
    
    const char *dirname = dataset.c_str();
    PyObject* py_dirname = PyUnicode_DecodeFSDefault(dirname);
    PyObject* py_tuple = PyTuple_Pack(1, py_dirname);
    PyObject_CallObject(py_initFunc, py_tuple);

    // Set a reference to the evaluation function
    py_evaluationFunction = PyObject_GetAttrString(module, "evaluate");
    if(!py_evaluationFunction){
        PyErr_Print();
        exit(-1);
    }
}

void ARK_Online::logAndSaveProgress(Individual &ind){
    JSON_Progress["total_evals"].push_back(totalEvaluations);
    JSON_Progress["unique_evals"].push_back(totalUniqueEvaluations);
    JSON_Progress["network_unique_evals"].push_back(totalNetworkUniqueEvaluations);
    JSON_Progress["evaluated_solution_genotype"].push_back(Utility::genotypeToString(ind.genotype));
    JSON_Progress["evaluated_solution_network"].push_back(networkLibrary.hash(ind.genotype));
    JSON_Progress["evaluated_solution_fitness"].push_back(ind.fitness);
    
    if (numObjectives == 1){
        JSON_Progress["best_solution_genotype"].push_back(Utility::genotypeToString(bestIndividual.genotype));
        JSON_Progress["best_solution_network"].push_back(networkLibrary.hash(bestIndividual.genotype));
        JSON_Progress["best_solution_fitness"].push_back(bestIndividual.fitness);
    } else {
        JSON_Progress["elitist_archive"].push_back(elitistArchiveToJSON());
    }
    
    Utility:: writeRawData(JSON_Progress.dump(), path_JSON_Progress);
}
