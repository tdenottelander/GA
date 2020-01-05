//
//  ARK2.hpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK2_hpp
#define ARK2_hpp

#include <stdio.h>
#include "ARK.hpp"

class ARK2 : public ARK {
public:
    ARK2();
    ARK2(int problemSize, bool allowIdentityLayers, int maxEvaluations);
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    float query(arma::uvec encoding) override;
    float query(std::vector<int> encoding) override;
    
    void setProblemType() override;
    void setProblemType(bool allowIdentityLayers);
    void setLength (int length) override;
};

static float ark2_optima[] = {31.8, 47.51, 57.23, 66.84, 76.3, 81.01, 85.19, 87.11, 88.63, 90.03, 90.98, 91.06, 91.44, 91.44};


#endif /* ARK2_hpp */
