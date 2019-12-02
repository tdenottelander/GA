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
    float optimum;
    bool optimumFound;
    int evaluations;
    
    FitnessFunction(int optimum);
    FitnessFunction();
    
    virtual float evaluate(Individual &ind) = 0;
    virtual void display();
    virtual std::string id();
    
    void checkIfBestFound(Individual &ind);
    
    virtual FitnessFunction* clone() const = 0;
    
    void setLength (int length);
};

class OneMax : public FitnessFunction {
public:
    OneMax (int length);
    OneMax ();
    float evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
};


class LeadingOnes : public FitnessFunction {
public:
    LeadingOnes (int length);
    LeadingOnes ();
    float evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
};


class TrapFive : public FitnessFunction {
public:
    int blocks;
    int k;
    TrapFive (int blocks);
    TrapFive ();
    float evaluate(Individual &ind) override;
    float subfunc (Individual &ind, int startIdx, int endIdx);
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    void setLength (int length);
};

#endif /* FitnessFunction_hpp */
