//
//  NK.hpp
//  GA
//
//  Created by Tom den Ottelander on 14/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef NK_hpp
#define NK_hpp

#include <stdio.h>
#include <unordered_map>
#include "FitnessFunction.hpp"

class NK : public FitnessFunction {
public:
    int blocksize;
    bool wraparound;
    std::unordered_map<std::string, float> lookupTable;
    NK();
    NK(int problemLength, int blocksize, bool wraparound, int alphabetsize, int maxEvaluations);
    void createTable();
    float evaluate(Individual &ind) override;
    ProblemType* getProblemType(int alphabetsize);
    
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    void setLength (int length) override;
};

#endif /* NK_hpp */
