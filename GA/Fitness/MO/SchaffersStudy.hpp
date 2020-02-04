//
//  SchaffersStudy.hpp
//  GA
//
//  Created by Tom den Ottelander on 04/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef SchaffersStudy_hpp
#define SchaffersStudy_hpp

#include <stdio.h>
#include "FitnessFunction.hpp"

class SchaffersStudy : public FitnessFunction {
public:
    SchaffersStudy();
    std::vector<float> evaluate (Individual &ind) override;
    
    FitnessFunction* clone() const override;
};

#endif /* SchaffersStudy_hpp */
