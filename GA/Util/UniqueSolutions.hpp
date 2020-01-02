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
    std::unordered_set<int> genotypes;
    void put(arma::uvec &genotype);
    bool contains(arma::uvec &genotype);
};

class SolutionCounter {
public:
    SolutionCounter(int alphabetSize);
    int alphabetSize;
    std::unordered_map<int, int> counterMap;
    void put(arma::uvec &genotype);
    bool contains(arma::uvec &genotype);
    int get(arma::uvec &genotype);
    nlohmann::json toJson (bool asHash);
};

namespace HashingFunctions {
    int hash(arma::uvec &genotype, int alphabetSize);
    arma::uvec decode(int hash, int problemSize, int alphabetSize);
};

#endif /* UniqueSolutions_hpp */
