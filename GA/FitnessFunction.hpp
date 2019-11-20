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
    OneMax (int optimum);
    int evaluate(Individual &ind) override;
    void display() override;
};


class LeadingOnes : public FitnessFunction {
public:
    LeadingOnes (int optimum);
    int evaluate(Individual &ind) override;
    void display() override;
};

#endif /* FitnessFunction_hpp */
