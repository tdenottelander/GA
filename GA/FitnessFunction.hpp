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
    Individual * bestIndividual;
    
    FitnessFunction();
    
    virtual int evaluate(Individual &ind);
    virtual void display();
};

class OneMax : public FitnessFunction {
public:
    OneMax ();
    int evaluate(Individual &ind) override;
    void display() override;
};

#endif /* FitnessFunction_hpp */
