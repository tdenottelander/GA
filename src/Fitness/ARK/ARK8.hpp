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

    // For normalizing the MMACs objective
    float minMMACs = 21.31;
    float maxMMACs = 239.28;
    
    std::vector<float> evaluate (Individual &ind) override;
    std::vector<float> getFitness (std::vector<int> encoding) override;
    
    void display() override;
    std::string id() override;
    FitnessFunction* clone() const override;
    
    ProblemType* getProblemType();
};

#endif /* ARK8_hpp */
