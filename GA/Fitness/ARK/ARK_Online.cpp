//
//  ARK_Online.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK_Online.hpp"

using namespace std;

ARK_Online::ARK_Online () : FitnessFunction(getProblemType()) {
    pythonInit();
}

vector<float> ARK_Online::evaluate(Individual &ind){
    
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
    
    // Unpack the result
    float validationAccuracy = PyFloat_AsDouble(PyTuple_GetItem(result, 0));
    int evaluations = PyLong_AsLong(PyTuple_GetItem(result, 1));

    // Print the result
    cout << "Retrieved values: " << validationAccuracy << ", " << evaluations << endl;
    
    vector<float> fitness = {validationAccuracy};
    ind.fitness = fitness;
    
    evaluationProcedure(ind);
    
    return fitness;
}

void ARK_Online::display(){
    cout << "ARK_Online fitness function" << endl;
}

string ARK_Online::id(){
    return ("ARK_Online");
}

ProblemType* ARK_Online::getProblemType(){
    vector<int> alphabet = {0,1,2};
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
    
    // Add path to script to sysPath
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Append(sysPath, PyUnicode_FromString("/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/nasbench/"));
    
    // Import python module/script
    module = PyImport_ImportModule("ark_online");
    if(module == NULL){
        printf("ERROR importing module 'ark_online'\n");
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
