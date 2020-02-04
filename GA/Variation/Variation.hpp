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
#include "FOS.hpp"

class Variation {
public:
    virtual std::vector<Individual> variate(std::vector<Individual> &population) = 0;
    virtual std::pair<Individual, Individual> crossover(Individual &ind1, Individual &ind2);
    virtual void display();
    virtual std::string id();
};

class UnivariateCrossover : public Variation {
public:
    UnivariateCrossover();
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    std::pair<Individual, Individual> crossover(Individual &ind1, Individual &ind2) override;
    void display() override;
    std::string id() override;
};

class OnePointCrossover : public Variation {
public:
    OnePointCrossover();
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    std::pair<Individual, Individual> crossover(Individual &ind1, Individual &ind2) override;
    void display() override;
    std::string id() override;
};

class GOM_Variation : public Variation {
public:
    FitnessFunction *fitfunc;
    Individual *bestIndividual;
    bool forcedImprovement;
    std::vector<arma::uvec> fos;
    FOS *fosObject;
    GOM_Variation(bool forcedInprovement = false);
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    Individual gom(Individual &ind, std::vector<Individual> &population, int indIdx);
    bool gomWithEliteIndividual(Individual &o, Individual &b);
    void applyDonor(Individual &ind, Individual &parent, arma::uvec &subset);
    void setFOSObject(FOS *f);
    void display() override;
    std::string id() override;
};

#endif /* Variation_hpp */
