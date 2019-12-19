//
//  Trap.hpp
//  GA
//
//  Created by Tom den Ottelander on 11/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef Trap_hpp
#define Trap_hpp

#include <stdio.h>
#include "FitnessFunction.hpp"

class Trap : public FitnessFunction {
public:
    int blocks;
    int blocksize;
    Trap (int blocksize, int blocks, int maxEvaluations);
    Trap (int blocksize, int maxEvaluations);
    float evaluate(Individual &ind) override;
    float subfunc (Individual &ind, int startIdx, int endIdx);
    void display() override;
    std::string id() override;
    void setProblemType() override;
    FitnessFunction* clone() const override;
    void setLength (int length) override;
};

#endif /* Trap_hpp */
