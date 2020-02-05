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
    std::vector<float> fitness;
    int counterNotChanged;
    
    // MO-fields
    int dominationCount;
    std::vector<Individual*> dominationList;
    int front;
    float crowdingDistance;
    bool canReproduce;
    
    Individual();
    Individual(int length, int objectives);
    void initialize(std::vector<int> alphabet);
    Individual copy();
    std::string toString();
    std::string toStringBlocks(int blocksize);
    bool equals(Individual const& ind);
    bool genotypeEquals(arma::uvec &genotype);
    bool fitnessEquals(Individual &ind, float margin = 0.001);
    bool dominates(Individual &ind2);
    void clearMOinformation();
    
    static std::string toString(std::vector<int> &genotype);
    static std::string toString(arma::uvec &genotype);
    static int hammingDistance(arma::uvec &g1, arma::uvec &g2);
    static int editDistance(arma::uvec &g1, arma::uvec &g2);
    static int editDistance(arma::uvec &g1, arma::uvec &g2, int i, int j);
    static arma::uvec removeIdentities(arma::uvec &genotype, int identityLayerIndex);
    static bool genotypeEquals(arma::uvec &g1, arma::uvec &g2);
    static bool dominates(Individual &ind1, Individual &ind2);
};

#endif /* Individual_hpp */


