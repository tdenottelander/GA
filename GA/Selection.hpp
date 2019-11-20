//
//  Selection.hpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef Selection_hpp
#define Selection_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Individual.hpp"
#include "Utility.hpp"

class Selection {
public:
    Selection();
    virtual std::vector<Individual> select(std::vector<Individual> &population, int outputSize) = 0;
    virtual void display();
};

class ProportionateSelection : public Selection {
public:
    ProportionateSelection();
    std::vector<Individual> select(std::vector<Individual> &population, int outputSize);
    void display() override;
    std::vector<float> getProportions(std::vector<Individual> &population);
};

class TournamentSelection : public Selection {
public:
    void select(std::vector<Individual> &population, int outputSize, int tournamentSize);
    void display() override;
};

#endif /* Selection_hpp */
