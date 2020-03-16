//
//  MO_RS.cpp
//  GA
//
//  Created by Tom den Ottelander on 26/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "MO_RS.hpp"

using namespace std;

MO_RS::MO_RS (FitnessFunction * fitFunc) : GA(fitFunc){
    preventIMS = true;
    isRandomSearchAlgorithm = true;
}

void MO_RS::round(){
    vector<int> alphabet = fitFunc_ptr->problemType->alphabet;
    
    for (int i = 0; i < population.size(); i++){
        population[i].initialize(alphabet);
        fitFunc_ptr->evaluate(population[i]);
        fitFunc_ptr->updateElitistArchive(&population[i]);
        if(fitFunc_ptr->isDone()){
            break;
        }
    }
    
    roundsCount++;
}

GA* MO_RS::clone() const {
    return new MO_RS(static_cast<const MO_RS&>(*this));
}

string MO_RS::id() {
    return "MO-RandomSearch";
}
