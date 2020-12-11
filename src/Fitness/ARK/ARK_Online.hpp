//
//  ARK_Online.hpp
//  GA
//
//  Created by Tom den Ottelander on 28/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK_Online_hpp
#define ARK_Online_hpp

#include <stdio.h>
#include <vector>
//#include <Python.h>
#include "FitnessFunction.hpp"

//static PyObject *module;
//static PyObject *py_evaluationFunction;

class ARK_Online : public FitnessFunction {
public:
    // For normalizing the MMACs objective
    float minMMACs = 13.03;
    float maxMMACs = 255.86;

    ARK_Online(int problemSize, int numberOfObjectives);
    std::vector<float> evaluate(Individual &ind) override;
//    std::vector<float> pyEvaluate(Individual &ind);
    void display() override;
    std::string id() override;
    ProblemType* getProblemType();
    FitnessFunction* clone() const override;
    void setLength (int length) override;
    
    void logAndSaveProgress(Individual &ind);
    
//    static void pythonInit();
};

#endif /* ARK_Online_hpp */
