//
//  Variation.hpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef Variation_hpp
#define Variation_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <armadillo>
#include "Individual.hpp"
#include "Utility.hpp"
#include "FitnessFunction.hpp"

class Variation {
public:
    virtual std::vector<Individual> variate(std::vector<Individual> &population) = 0;
    virtual void display();
    virtual std::string id();
};

class UnivariateCrossover : public Variation {
public:
    UnivariateCrossover();
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    std::pair<Individual, Individual> univariateCrossover(Individual &ind1, Individual &ind2);
    void display() override;
    std::string id() override;
};

class OnePointCrossover : public Variation {
public:
    OnePointCrossover();
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    std::pair<Individual, Individual> onePointCrossover(Individual &ind1, Individual &ind2);
    void display() override;
    std::string id() override;
};

class GOM : public Variation {
public:
    FitnessFunction *fitfunc;
    std::vector<arma::uvec> FOS;
    GOM();
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    Individual gom(Individual &ind, std::vector<Individual> &population);
    void applyDonor(Individual &ind, Individual &parent, arma::uvec &subset);
    static std::vector<arma::uvec> getFixedLTFOS(int n);
    void display() override;
    std::string id() override;
};

#endif /* Variation_hpp */
