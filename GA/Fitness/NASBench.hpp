//
//  NASBench.hpp
//  GA
//
//  Created by Tom den Ottelander on 12/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef NASBench_hpp
#define NASBench_hpp

#include <stdio.h>
#include <vector>
#include <Python.h>
#include "FitnessFunction.hpp"

static PyObject *module;
static PyObject *py_queryfunc;

class NASBench : public FitnessFunction {
public:
    NASBench(PyObject *module, PyObject *py_queryfunc);
    float evaluate(Individual &ind) override;
    std::string intToLayer(int encoding);
    void display() override;
    std::string id() override;
    void setProblemType() override;
    FitnessFunction* clone() const override;
    void setLength (int length) override;
    
    static void pythonInit();
};

#endif /* NASBench_hpp */
