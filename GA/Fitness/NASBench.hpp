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

extern PyObject *module;

class NASBench : public FitnessFunction {
public:
//    PyObject *module;
    PyObject *py_queryfunc;
    NASBench(PyObject *module, PyObject *py_queryfunc);
    float evaluate(Individual &ind) override;
    std::string intToLayer(int encoding);
    void display() override;
    std::string id() override;
    void setProblemType() override;
    FitnessFunction* clone() const override;
    void setLength (int length) override;
};

#endif /* NASBench_hpp */
