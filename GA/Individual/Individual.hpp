//
//  Individual.hpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef Individual_hpp
#define Individual_hpp

#include <stdio.h>
#include <armadillo>
#include <string>
#include <vector>
#include "Utility.hpp"

class Individual {
public:
    arma::uvec genotype;
    float fitness;
    int counterNotChanged;
    
    Individual();
    Individual(int length);
    void initialize(std::vector<int> alphabet);
    Individual copy();
    std::string toString();
    std::string toStringBlocks(int blocksize);
    bool equals(Individual const& ind);
    
    static std::string toString(std::vector<int> genotype);
    static std::string toString(arma::uvec genotype);
};

#endif /* Individual_hpp */


