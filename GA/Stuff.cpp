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


void Stuff::pythonScript () {
    //    Py_Initialize();
    //
    //    //setting the path to .py file
    //    sprintf(module_name, "import sys; sys.path.insert(0, \"%s/\")", pwd);
    //    printf("%s\n", module_name);
    
    Py_Initialize();
    
    //    PyRun_SimpleString("print('hello world')");
    //    //    PyRun_SimpleString ("import sys; print sys.path");
    //
    //    //uploading .py module
    
//    string filename = "./module.py";
//    PyObject* sysPath = PySys_GetObject((char*)"path");
    //    cout << sysPath << endl;
    //    PyObject* programName = PyUnicode_FromString(filename.c_str());
    
    //    PyImport_ImportModule(filename.c_str());
    
    PyObject *module = PyImport_ImportModule("python_functions");
    if (module == NULL)
    {
        printf("ERROR importing module python functions");
        exit(-1);
    }
    //
    //    //finding function_init function from the uploaded module
    //    PyObject *function_init = PyObject_GetAttrString(module,(char*)"function_init");
    //    if (function_init == NULL)
    //    {
    //        printf("ERROR getting function init");
    //        exit(-1);
    //    }
    //
    //
    //    //initializing an array as python function input
    //    PyObject* pyParameters = PyList_New(number_of_parameters);
    //    if (pyParameters == NULL)
    //    {
    //        printf("ERROR creating args array");
    //        exit(-1);
    //    }
    //
    //    //copying values from C array to python one
    //    for (int i = 0; i < number_of_parameters; ++i)
    //        PyList_SetItem(pyParameters, i, PyFloat_FromDouble((double)parameters[i]));
    //
    //
    //    //adding an integer argument for python function
    //    PyObject *pyOption = Py_BuildValue("i", option);
    //
    //    //packing two inputs (array and integer) together
    //    PyObject *pyArgs = PyTuple_Pack(2, pyParameters, pyOption);
    //
    //    //calling function
    //    PyObject* pyResult = PyObject_CallObject(function, pyArgs);
    //
    //    //getting first field of the output
    //    PyObject *pyvalue = PyTuple_GetItem(pyResult, 0);
    //
    //    //converting the output to C
    //    *value = PyFloat_AsDouble(pyvalue);
    //
    Py_Finalize();
}

void Stuff::pythonScript2(){
    // Get a reference to the main module.
    //    PyObject* main_module = PyImport_AddModule("__main__");
    
    // Get the main module's dictionary
    // and make a copy of it.
    //    PyObject* main_dict = PyModule_GetDict(main_module);
    //    PyObject* main_dict_copy = PyDict_Copy(main_dict);
    
    // Execute two different files of
    // Python code in separate environments
    
    //    PyRun_File(file_1, "module.py",
    //               Py_file_input,
    //               main_dict, main_dict);
    
    //    PyRun_AnyFile(file_1, "$HOME/module.py");
    
    //    FILE* file_2 = fopen("file2.py", "r");
    //    PyRun_File(file_2, "file2.py",
    //               Py_file_input,
    //               main_dict_copy, main_dict_copy);
    
}

void Stuff::pythonScript3(){
    Py_Initialize();
    
    int number_of_parameters = 2;
    
    //initializing an array as python function input
    PyObject* pyParameters = PyList_New(number_of_parameters);
    if (pyParameters == NULL)
    {
        printf("ERROR creating args array");
        exit(-1);
    }
    
    //copying values from C array to python one
    //    for (int i = 0; i < number_of_parameters; ++i)
    //        PyList_SetItem(pyParameters, i, PyFloat_FromDouble((double)parameters[i]));
    
    //adding an integer argument for python function
    PyObject *pyOption = Py_BuildValue("i", optind);
    
    //packing two inputs (array and integer) together
    PyObject *pyArgs = PyTuple_Pack(2, pyParameters, pyOption);
    
    //calling function
    //    PyObject* pyResult = PyObject_CallObject(function, pyArgs);
    
    //getting first field of the output
    //    PyObject *pyvalue = PyTuple_GetItem(pyResult, 0);
    
    //converting the output to C
    //    *value = PyFloat_AsDouble(pyvalue);
    
    Py_Finalize();
}

void Stuff::pythonScript4(){
    string filename = projectsDir + "GA/GA/module.py";
    
    PyObject* sysPath = PySys_GetObject((char*)"path");
    //    PyObject* programName = PyString_FromString(SplitFilename(argv[1]).c_str());
    
    //    PyObject *pModule = PyImport_Import(<#PyObject *name#>)
}

void Stuff::sanityCheck(){
    string filename = projectsDir + "GA/GA/test.txt";
    FILE* file = fopen(filename.c_str(), "r");
    int c;
    if (file) {
        while ((c = getc(file)) != EOF)
            putchar(c);
        fclose(file);
    }
}
