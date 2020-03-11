//
//  ARK_Online.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK_Online.hpp"

using namespace std;

extern nlohmann::json JSON_MO_info;
extern nlohmann::json JSON_SO_info;
extern nlohmann::json JSON_Progress;
extern string progressWritePath;

ARK_Online::ARK_Online (int problemSize, int numberOfObjectives) : FitnessFunction(getProblemType()), networkLibrary(SolutionLibrary::Type::ARK_ONLINE) {
    numObjectives = numberOfObjectives;
    totalProblemLength = problemSize;
    storeNetworkUniqueEvaluations = true;
    pythonInit();
}

vector<float> ARK_Online::evaluate(Individual &ind){
    
//    bool networkunique = false;
    vector<float> fitness;
    
//    cout << ind.toString() << endl;
    if (networkLibrary.contains(ind.genotype)){
        fitness = networkLibrary.get(ind.genotype);
    } else {
    
    fitness = pyEvaluate(ind);
        
        // DUMMY EVALUATION:
//        if (numObjectives == 1){
//            fitness = {(float)Utility::getRand()};
//        } else {
//            fitness = {(float)Utility::getRand(), (float)Utility::getRand()};
//        }
        
        
        networkLibrary.put(ind.genotype, fitness);
        totalNetworkUniqueEvaluations++;
//        networkunique = true;
    }
    
    ind.fitness = fitness;
    
    evaluationProcedure(ind);

    if(log(totalNetworkUniqueEvaluations)){
        if (numObjectives == 1){
            JSON_SO_info["changes_on_interval"]["network_unique_evals"]["best_solution_genotype"].push_back(Utility::genotypeToString(bestIndividual.genotype));
            JSON_SO_info["changes_on_interval"]["network_unique_evals"]["best_solution_fitness"].push_back(bestIndividual.fitness[0]);
            JSON_SO_info["changes_on_interval"]["network_unique_evals"]["evals"].push_back(totalNetworkUniqueEvaluations);
        } else {
            pair<float, float> avg_max_distance = calculateDistanceParetoToApproximation();
            JSON_MO_info["changes_on_interval"]["network_unique_evals"]["elitist_archive_fitness"].push_back(elitistArchiveToJSON());
            JSON_MO_info["changes_on_interval"]["network_unique_evals"]["avg_dist"].push_back(avg_max_distance.first);
            JSON_MO_info["changes_on_interval"]["network_unique_evals"]["max_dist"].push_back(avg_max_distance.second);
            JSON_MO_info["changes_on_interval"]["network_unique_evals"]["evals"].push_back(totalNetworkUniqueEvaluations);
            JSON_MO_info["changes_on_interval"]["network_unique_evals"]["pareto_points_found"].push_back(paretoPointsFound());
        }
    }
    
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
    
    Utility:: writeRawData(JSON_Progress.dump(), progressWritePath);
    
//    cout << (networkunique ? "eval: " : "lib:  ") << ind.toString() << " evals: " << totalEvaluations << " uniqEvals: " << totalUniqueEvaluations << " netUniqEvals: " << totalNetworkUniqueEvaluations << endl;
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
    return ("ARK-Online-" + to_string(numObjectives));
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
//    PyList_Append(sysPath, PyUnicode_FromString("/export/scratch1/tdo/TomGA/"));
    PyList_Append(sysPath, PyUnicode_FromString("/export/scratch1/tdo/nas_online/"));

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
    
    PyObject_CallObject(py_initFunc, NULL);

    // Set a reference to the evaluation function
    py_evaluationFunction = PyObject_GetAttrString(module, "evaluate");
    if(!py_evaluationFunction){
        PyErr_Print();
        exit(-1);
    }
}

void ARK_Online::clear() {
    networkLibrary.clear();
    FitnessFunction::clear();
}
