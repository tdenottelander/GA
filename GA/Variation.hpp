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

class Variation {
public:
    virtual std::vector<Individual> variate(std::vector<Individual> &population) = 0;
    virtual void display();
};

class UnivariateCrossover : public Variation {
public:
    UnivariateCrossover();
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    std::pair<Individual, Individual> univariateCrossover(Individual &ind1, Individual &ind2);
    void display() override;
};

class OnePointCrossover : public Variation {
public:
    OnePointCrossover();
    std::vector<Individual> variate(std::vector<Individual> &population) override;
    std::pair<Individual, Individual> onePointCrossover(Individual &ind1, Individual &ind2);
    void display() override;
};

#endif /* Variation_hpp */
