//
//  ARK4.hpp
//  GA
//
//  Created by Tom den Ottelander on 13/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK4_hpp
#define ARK4_hpp

#include <stdio.h>
#include "ARK.hpp"

class ARK4 : public ARK {
public:
    ARK4();
    ARK4(int problemSize, bool allowIdentityLayers);
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;

    ProblemType* getProblemType(bool allowIdentityLayers);
};

//static float ark4_optima[] = {54.51, 75.01, 83.53, 86.21, 87.87, 87.87, 89.26, 90.18, 90.83, 91.51, 91.81, 92.17};

#endif /* ARK4_hpp */
