//
//  ARK5.hpp
//  GA
//
//  Created by Tom den Ottelander on 14/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK5_hpp
#define ARK5_hpp

#include <stdio.h>
#include "ARK.hpp"

class ARK5 : public ARK {
public:
    ARK5();
    ARK5(int problemSize, bool allowIdentityLayers, int maxEvaluations);
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    ProblemType* getProblemType(bool allowIdentityLayers);
    void setLength (int length) override;
};

static float ark5_optima[] = {54.51, 75.01, 83.53, 86.21, 87.87, 87.87, 89.26, 90.18, 90.83, 91.51};

#endif /* ARK5_hpp */
