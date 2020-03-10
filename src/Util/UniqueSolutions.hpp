//
//  UniqueSolutions.hpp
//  GA
//
//  Created by Tom den Ottelander on 18/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef UniqueSolutions_hpp
#define UniqueSolutions_hpp

#include <stdio.h>
#include <armadillo>
#include <unordered_set>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Utility.hpp"

class UniqueSolutions{
public:
    UniqueSolutions(int alphabetSize);
    int alphabetSize;
    std::unordered_map<std::string, std::vector<float>> genotypes;
    void put(arma::uvec &genotype, std::vector<float> &fitness);
    bool contains(arma::uvec &genotype);
    std::vector<float> get(arma::uvec &genotype);
};

class SolutionCounter {
public:
    SolutionCounter(int alphabetSize, int problemSize);
    int problemSize;
    int alphabetSize;
    std::unordered_map<int, int> counterMap;
    void put(arma::uvec &genotype);
    bool contains(arma::uvec &genotype);
    long get(arma::uvec &genotype);
    nlohmann::json toJson (bool asHash);
};

namespace HashingFunctions {
    long hash(arma::uvec &genotype, int alphabetSize);
    arma::uvec decode(long hash, int problemSize, int alphabetSize);
    std::string toString(arma::uvec &genotype, std::string type="default");
    arma::uvec toGenotype(std::string str);
};

#endif /* UniqueSolutions_hpp */
