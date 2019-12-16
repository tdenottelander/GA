//
//  NASBench.cpp
//  GA
//
//  Created by Tom den Ottelander on 12/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "NASBench.hpp"

using namespace std;

NASBench::NASBench () : FitnessFunction(1.0) {
    setProblemType();
}

float NASBench::evaluate(Individual &ind){
    
    //Query solution here (together with sequential neural network structure.
    PyObject* py_args = Py_BuildValue("[iiiii]", ind.genotype[0], ind.genotype[1], ind.genotype[2], ind.genotype[3], ind.genotype[4]);
    PyObject* py_tuple = PyTuple_Pack(1, py_args);
    if(!py_queryfunc){
        PyErr_Print();
        exit(-1);
    }
    PyObject* result = PyObject_CallObject(py_queryfunc, py_tuple);
    
    //Return the validation accuracy of some epoch budget.
    float validationAccuracy = PyFloat_AsDouble(result);
//    cout << validationAccuracy << endl;
    
    ind.fitness = validationAccuracy;
    
    evaluation(ind);
    
    return validationAccuracy;
}

string NASBench::intToLayer(int encoding){
    switch (encoding){
        case 0: return "CONV1X1";
        case 1: return "CONV3X3";
        case 2: return "MAXPOOL3X3";
    }
    return "ERROR";
}

void NASBench::display(){
    cout << "NASBench fitness function" << endl;
}

string NASBench::id(){
    return ("NASBench");
}

void NASBench::setProblemType(){
    vector<int> alphabet = {0,1,2};
    FitnessFunction::setProblemType(new AlphabetProblemType(alphabet));
}

FitnessFunction* NASBench::clone() const {
    return new NASBench(static_cast<const NASBench&>(*this));
}

void NASBench::setLength (int length){
    totalProblemLength = length;
}

void NASBench::pythonInit(){
    Py_Initialize();
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyList_Append(sysPath, PyUnicode_FromString("/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/nasbench/"));
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
    
    py_queryfunc = PyObject_GetAttrString(module, "query");
    if(!py_queryfunc){
        PyErr_Print();
        exit(-1);
    }
}
