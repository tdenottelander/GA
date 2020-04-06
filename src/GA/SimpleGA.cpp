//
//  SimpleGA.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "SimpleGA.hpp"
using namespace std;

SimpleGA::SimpleGA (FitnessFunction * fitFunc, Variation * var, Selection * sel) : GA(fitFunc) {
    variation_ptr = var;
    selection_ptr = sel;
}


void SimpleGA::roundPOVariationSelection(){
    vector<Individual> offspring = variation_ptr->variate(population);
    mutate(offspring);
    
    evaluateAll(offspring);
    
    vector<Individual> PO;
    PO.reserve(population.size() * 2);
    PO.insert(PO.begin(), population.begin(), population.end());
    PO.insert(PO.end(), offspring.begin(), offspring.end());
    
    population = selection_ptr->select(PO, population.size());
}

void SimpleGA::round(){
    roundPOVariationSelection();
    roundsCount++;
}

GA* SimpleGA::clone() const {
    return new SimpleGA(static_cast<const SimpleGA&>(*this));
}

string SimpleGA::id() {
    return "SimpleGA_s=" + selection_ptr->id() + "_v=" + variation_ptr->id();
}

void SimpleGA::mutate(vector<Individual> &pop) {
    float chanceForMutation = 1.0f / fitFunc_ptr->totalProblemLength;
    for (int i = 0; i < pop.size(); i++){
        for (int j = 0; j < pop[i].genotype.size(); j++){
            if (Utility::getRand() < chanceForMutation){
                int originalBit = pop[i].genotype[j];
                int newBit = originalBit;
                do {
                    newBit = Utility::getRand(0, fitFunc_ptr->problemType->alphabet.size());
                } while (originalBit == newBit);
                pop[i].genotype[j] = newBit;
            }
        }
    }
}
