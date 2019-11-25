//
//  FitnessFunction.hpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef FitnessFunction_hpp
#define FitnessFunction_hpp

#include <stdio.h>
#include "Individual.hpp"

class FitnessFunction {
public:
    Individual bestIndividual;
    int optimum;
    bool optimumFound;
    
    FitnessFunction(int optimum);
    
    virtual int evaluate(Individual &ind) = 0;
    virtual void display();
    
    void checkIfBestFound(Individual &ind);
};

class OneMax : public FitnessFunction {
public:
    OneMax (int length);
    int evaluate(Individual &ind) override;
    void display() override;
};


class LeadingOnes : public FitnessFunction {
public:
    LeadingOnes (int length);
    int evaluate(Individual &ind) override;
    void display() override;
};


class TrapFive : public FitnessFunction {
public:
    int blocks;
    int k;
    TrapFive (int blocks);
    float evaluate(Individual &ind) override;
    float subfunc (Indiviudal &ind, int startIdx, int endIdx);
    void display() override;
};

#endif /* FitnessFunction_hpp */
