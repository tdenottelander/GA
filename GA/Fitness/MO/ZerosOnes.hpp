//
//  ZerosOnes.hpp
//  GA
//
//  Created by Tom den Ottelander on 04/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ZerosOnes_hpp
#define ZerosOnes_hpp

#include <stdio.h>
#include "FitnessFunction.hpp"

class ZerosOnes : public FitnessFunction {
public:
    ZerosOnes(int problemSize);
    std::vector<float> evaluate (Individual &ind) override;
    
    FitnessFunction* clone() const override;
    void setOptimum(std::vector<float> optimum) override;
    bool entireParetoFrontFound() override;
};

#endif /* ZerosOnes_hpp */
