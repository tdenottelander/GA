//
//  ARK3.hpp
//  GA
//
//  Created by Tom den Ottelander on 07/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK3_hpp
#define ARK3_hpp

#include <stdio.h>
#include "ARK.hpp"

class ARK3 : public ARK {
public:
    ARK3();
    ARK3(int maxEvaluations);
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    ProblemType* getProblemType();
    void setLength (int length) override;
};

static float ark3_optimum = 92.25;

#endif /* ARK3_hpp */
