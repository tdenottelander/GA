//
//  RandomSearch.cpp
//  GA
//
//  Created by Tom den Ottelander on 16/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "RandomSearch.hpp"

using namespace std;

RandomSearch::RandomSearch (FitnessFunction * fitFunc) : GA(fitFunc){}

void RandomSearch::round(){
    GA::initializeTrueRandomPopulation();
    roundsCount++;
}

GA* RandomSearch::clone() const {
    return new RandomSearch(static_cast<const RandomSearch&>(*this));
}

string RandomSearch::id() {
    return "RandomSearch";
}
