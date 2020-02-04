//
//  ZDT2.hpp
//  GA
//
//  Created by Tom den Ottelander on 04/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ZDT2_hpp
#define ZDT2_hpp

#include <stdio.h>
#include "FitnessFunction.hpp"

class ZDT2 : public FitnessFunction {
public:
    ZDT2();
    std::vector<float> evaluate (Individual &ind) override;
    
    FitnessFunction* clone() const override;
};


#endif /* ZDT2_hpp */
