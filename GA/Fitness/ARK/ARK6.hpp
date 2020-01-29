//
//  ARK6.hpp
//  GA
//
//  Created by Tom den Ottelander on 23/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK6_hpp
#define ARK6_hpp

#include <stdio.h>
#include "ARK.hpp"
#include <algorithm>

class ARK6 : public ARK {
public:
    ARK6(int problemSize, bool genotypeChecking);
        
    float getFitness (arma::uvec encoding) override;
    
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    ProblemType* getProblemType();
};

#endif /* ARK6_hpp */
