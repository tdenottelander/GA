//
//  CountingOnesMO.hpp
//  GA
//
//  Created by Tom den Ottelander on 04/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef CountingOnesMO_hpp
#define CountingOnesMO_hpp

#include <stdio.h>
#include "FitnessFunction.hpp"

class CountingOnesMO : public FitnessFunction {
public:
    CountingOnesMO(int blocksize, int numObjectives);
    int blocksize;
    std::vector<float> evaluate (Individual &ind) override;
    
    FitnessFunction* clone() const override;
    void setLength(int length) override;
};

#endif /* CountingOnesMO_hpp */
