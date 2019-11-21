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
#include "Utility.hpp"

class Individual {
public:
    arma::uvec genotype;
    int fitness;
    
    Individual(int length);
    void initialize();
    Individual copy();
    std::string toString();
    bool operator==(Individual const& ind);
    bool operator!=(Individual const& ind);
};

#endif /* Individual_hpp */


