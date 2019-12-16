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
    static float query(arma::uvec encoding);
    static float query(std::vector<int> encoding);
    void display() override;
    std::string id() override;
    void setProblemType() override;
    FitnessFunction* clone() const override;
    void setLength (int length) override;
    
    static void greedyRun();
    static void greedyRunBackward();
    static void greedyBothWays();
    static void greedyInsideOut();
    static void findBest(int length);
    static void findBestRecursion(int length, std::vector<int> &temp, int idx);
    
    static void printArchitecture(std::vector<int> architecture);
};

static float bestSoFar = -1.0;


#endif /* NASBenchV2_hpp */
