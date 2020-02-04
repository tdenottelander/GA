//
//  SimpleMOProblem.hpp
//  GA
//
//  Created by Tom den Ottelander on 03/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef SimpleMOProblem_hpp
#define SimpleMOProblem_hpp

#include <stdio.h>
#include "FitnessFunction.hpp"

class SimpleMOProblem : public FitnessFunction {
public:
    SimpleMOProblem(int blocksize, int numObjectives);
    int blocksize;
    std::vector<float> evaluate (Individual &ind) override;
    
    FitnessFunction* clone() const override;
};

#endif /* SimpleMOProblem_hpp */
