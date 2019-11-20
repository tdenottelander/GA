//
//  FitnessFunction.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "FitnessFunction.hpp"

using namespace std;

/* ------------------------ Base Fitness Function ------------------------ */

FitnessFunction::FitnessFunction(int optimum) : optimum(optimum), bestIndividual(0), optimumFound(false){
}

void FitnessFunction::display(){
    cout << "Base fitness function" << endl;
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int optimum) : FitnessFunction(optimum) {}

int OneMax::evaluate(Individual &ind) {
    int result = 0;
    for(int i = 0; i < ind.genotype.size(); i++){
        result += ind.genotype[i];
    }
    ind.fitness = result;
    if(result > bestIndividual.fitness){
        bestIndividual = ind.copy();
        if(result == optimum){
            optimumFound = true;
        }
    }
    return result;
}

void OneMax::display() {
    cout << "OneMax fitness function" << endl;
}
