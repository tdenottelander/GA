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

void FitnessFunction::checkIfBestFound(Individual &ind){
    if(ind.fitness > bestIndividual.fitness){
        bestIndividual = ind.copy();
        if(ind.fitness == optimum){
            optimumFound = true;
        }
    }
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int optimum) : FitnessFunction(optimum) {}

int OneMax::evaluate(Individual &ind) {
    int result = sum(ind.genotype);
    ind.fitness = result;
    
    checkIfBestFound(ind);
    
    return result;
}

void OneMax::display() {
    cout << "OneMax fitness function" << endl;
}


/* ------------------------ Leading Ones Fitness Function ------------------------ */

LeadingOnes::LeadingOnes(int optimum) : FitnessFunction(optimum) {}

int LeadingOnes::evaluate(Individual &ind) {
    int result = 0;
    for (int i = 0; i < ind.genotype.size(); i++){
        int tempResult = 1;
        for (int j = 0; j < (i + 1); j++){
            tempResult *= ind.genotype[j];
        }
        result += tempResult;
    }
    ind.fitness = result;
    
    checkIfBestFound(ind);
    
    return result;
}

void LeadingOnes::display() {
    cout << "LeadingOnes fitness function" << endl;
}
