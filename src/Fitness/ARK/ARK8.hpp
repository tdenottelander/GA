//
//  ARK8.hpp
//  GA
//
//  Created by Tom den Ottelander on 09/03/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef ARK8_hpp
#define ARK8_hpp

#include <stdio.h>
#include "ARK.hpp"
#include <algorithm>

class ARK8 : public ARK {
public:
    ARK8(int problemSize, bool genotypeChecking, bool MO = false);
    bool MO = false;
    std::vector<float> normalization = {1.0f/100.0f, 1.0f/500.0f};
    
    std::vector<float> evaluate (Individual &ind) override;
    std::vector<float> getFitness (std::vector<int> encoding) override;
    
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    ProblemType* getProblemType();
};

#endif /* ARK8_hpp */
