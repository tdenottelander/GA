//
//  Stuff.cpp
//  GA
//
//  Created by Tom den Ottelander on 02/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "Stuff.hpp"

using namespace Utility;
using namespace arma;
using namespace std;

const string projectsDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/";

int Stuff::mainLoop(){
    long begin = millis();
    
    int l = 100;
    int n = 150;
    int maxGenerations = 50;
    int runs = 100;
    
    int optFoundCounter = 0;
    
    for (int run = 0; run < runs; run++){
        bool optFound = false;
        cout << "run " << run+1 << ". ";
        
        TournamentSelection selection(4);
        UnivariateCrossover variation;
        OneMax fitnessFunc(l);
        
        //        GOM_Variation gom_var;
        //        FOS fos;
        
        //        GOM ga (&gom_var, &fos);
        GA ga;
        ga.setPointers(&fitnessFunc, &selection, &variation);
        ga.setPopulationSize(n);
        ga.setProblemLength(l);
        ga.initialize();
        //        cout << ga.toString() << endl;
        
        for(int gen = 0; gen < maxGenerations; gen++){
            ga.roundPOVariationSelection();
            //            cout << "\nRound " << j << "  | Avg fitness: " << ga.getAvgFitness() << "  | Best fitness: " << ga.fitFunc->bestIndividual.fitness << endl;
            //            cout << ga.toString() << endl;
            //            cout << "converged " << ga.isConverged() << endl;
            //            cout << "diverse " << ga.isDiverse() << endl;
            
            if(ga.fitFunc_ptr->optimumFound){
                optFound = true;
                cout << "Optimum found after " << gen << " generations" << endl;
                //                cout << "Individual: " << ga.fitFunc->bestIndividual.toString() << endl;
                break;
            }
        }
        
        if(!optFound){
            cout << "No optimum found" << endl;
        } else {
            optFoundCounter++;
        }
    }
    
    cout << "Optimal found " << optFoundCounter << " out of " << runs << " runs" << endl;
    
    long end = millis();
    cout << "Time taken: " << end - begin << "ms" << endl;
    
    return 0;
}


void Stuff::pythonSimpleString (){
    Py_Initialize();
    PyRun_SimpleString("print('hello world')");
    Py_Finalize();
}

PyObject* Stuff::loadPythonModule (){
    PyObject* sysPath = PySys_GetObject((char*)"path");
//    PyList_Append(sysPath, PyString_FromString("."));
    PyList_Append(sysPath, PyString_FromString("/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/GA/"));
    
    PyObject *module = PyImport_ImportModule("python_functions");
    if(module == NULL){
        printf("ERROR importing module 'python_functions'\n");
        exit(-1);
    }
    return module;
}

void Stuff::pythonSimpleFunction(){
    Py_Initialize();
    PyObject* module = loadPythonModule();
    
    PyObject* fooFunc = PyObject_GetAttrString(module, "foo");
    if(!fooFunc){
        PyErr_Print();
    }
    PyObject_CallObject(fooFunc, NULL);
    
    PyObject* barFunc = PyObject_GetAttrString(module, "bar");
    if(!barFunc) PyErr_Print();
    
    PyObject* result = PyObject_CallObject(barFunc, NULL);
    if(!result) PyErr_Print();
    
    int x = _PyInt_AsInt(result);
    cout << x << endl;
    
    PyObject* incrementFunc = PyObject_GetAttrString(module, "incrementInteger");
    if(!incrementFunc) PyErr_Print();
    PyObject_CallObject(incrementFunc, NULL);
    PyObject_CallObject(incrementFunc, NULL);
    PyObject_CallObject(incrementFunc, NULL);
    PyObject_CallObject(incrementFunc, NULL);
}

void Stuff::pythonArgumentFunction1(){
    Py_Initialize();
    PyObject* module = loadPythonModule();
    
    PyObject* foobarFunc = PyObject_GetAttrString(module, "foobar");
    PyObject* pyArg = Py_BuildValue("(i)", 1);
    PyObject* result = PyObject_CallObject(foobarFunc, pyArg);
    if(!result) PyErr_Print();

    int x = _PyInt_AsInt(result);
    cout << x << endl;
    
    
    PyObject* add = PyObject_GetAttrString(module, "add");
    PyObject* pyArgs = Py_BuildValue("(ii)", 5, 10);
    PyObject* resultAdd = PyObject_CallObject(add, pyArgs);
    if(!resultAdd) PyErr_Print();
    
    int y = _PyInt_AsInt(resultAdd);
    cout << y << endl;
}

void Stuff::pythonArgumentFunction2(){
    Py_Initialize();
    PyObject* module = loadPythonModule();
    
    PyObject* func = PyObject_GetAttrString(module, "listAppend");
    PyObject* pyArgList = Py_BuildValue("[iii]", 1, 2, 3);
    PyObject* pyArgInt = Py_BuildValue("i", 4);
    
    PyObject* pyArgs = PyTuple_Pack(2, pyArgList, pyArgInt);

    PyObject* result = PyObject_CallObject(func, pyArgs);
    if(!result) PyErr_Print();
    
    PyObject *pyvalue = PyList_GetItem(result, 3);
    cout << PyInt_AsLong(pyvalue) << endl;
}

////copying values from C array to python one
//for (int i = 0; i < number_of_parameters; ++i)
//    PyList_SetItem(pyParameters, i, PyFloat_FromDouble((double)parameters[i]));
//
////packing two inputs (array and integer) together
//PyObject *pyArgs = PyTuple_Pack(2, pyParameters, pyOption);
////getting first field of the output
//PyObject *pyvalue = PyTuple_GetItem(pyResult, 0);
