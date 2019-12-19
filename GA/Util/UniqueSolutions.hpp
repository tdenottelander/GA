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

class UniqueSolutions{
public:
    std::unordered_set<int> genotypes;
    int alphabetSize;
    UniqueSolutions(int alphabetSize);
    void put(arma::uvec &genotype);
    bool contains(arma::uvec &genotype);
    int hash(arma::uvec &genotype);
};

#endif /* UniqueSolutions_hpp */
