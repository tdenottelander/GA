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
    int n = fitFunc_ptr->totalProblemLength;
    vector<int> alphabet = fitFunc_ptr->problemType->alphabet;
    int alphLength = alphabet.size();
    for (int i = 0; i < population.size(); i++){
        for (int j = 0; j < n; j++){
            population[i].genotype[j] = alphabet[floor(Utility::getRand() * alphLength)];
        }
    }
    evaluateAll();
    roundsCount++;
}

GA* RandomSearch::clone() const {
    return new RandomSearch(static_cast<const RandomSearch&>(*this));
}

string RandomSearch::id() {
    return "RandomSearch";
}
