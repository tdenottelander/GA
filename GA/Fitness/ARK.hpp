//
//  ARK.hpp
//  GA
//
//  Created by Tom den Ottelander on 16/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef ARK_hpp
#define ARK_hpp

#include <stdio.h>
#include <vector>
#include <armadillo>
#include <nlohmann/json.hpp>
#include "FitnessFunction.hpp"
#include <unordered_map>


class ARK : public FitnessFunction {
public:
    ARK(int problemSize, int maxEvaluations, float optimum);
    float evaluate(Individual &ind) override;
    virtual float query(arma::uvec encoding);
    virtual float query(std::vector<int> encoding);
    void display() override;
    std::string id() override;
//    void setProblemType() override;
//    void setProblemType(bool allowIdentityLayers);
//    FitnessFunction* clone() const override;
    void setLength (int length) override;
    float getOptimum(int problemLength);
    
    static void printArchitecture(std::vector<int> architecture);
};

#endif /* ARK_hpp */
