//
//  NASBenchV2.hpp
//  GA
//
//  Created by Tom den Ottelander on 16/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef NASBenchV2_hpp
#define NASBenchV2_hpp

#include <stdio.h>
#include <vector>
#include <armadillo>
#include <nlohmann/json.hpp>
#include "FitnessFunction.hpp"


class NASBenchV2 : public FitnessFunction {
public:
    NASBenchV2();
    float evaluate(Individual &ind) override;
    float query(arma::uvec encoding);
    void display() override;
    std::string id() override;
    void setProblemType() override;
    FitnessFunction* clone() const override;
    void setLength (int length) override;
};

#endif /* NASBenchV2_hpp */
