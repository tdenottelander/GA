//
//  ARK1.hpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK1_hpp
#define ARK1_hpp

#include <stdio.h>
#include "ARK.hpp"

class ARK1 : public ARK {
public:
    ARK1();
    ARK1(int problemSize, bool allowIdentityLayers, int maxEvaluations);
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    ProblemType* getProblemType(bool allowIdentityLayers);
    void setLength (int length) override;
    
    arma::uvec transform (arma::uvec &genotype) override;
};

static float ark1_optima[] = {30.82, 55.36, 67.21, 78.0, 84.04, 86.15, 88.09, 89.14};


#endif /* ARK1_hpp */
