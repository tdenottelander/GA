//
//  ARK7.hpp
//  GA
//
//  Created by Tom den Ottelander on 29/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK7_hpp
#define ARK7_hpp

#include <stdio.h>



#include <stdio.h>
#include "ARK.hpp"
#include <algorithm>

class ARK7 : public ARK {
public:
    ARK7(int problemSize, bool genotypeChecking);
    
//    float query (arma::uvec encoding) override;
    float getFitness (arma::uvec encoding) override;
    
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    ProblemType* getProblemType();
};

#endif /* ARK7_hpp */
