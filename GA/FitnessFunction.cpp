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

string FitnessFunction::id() {
    return "base";
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int length) : FitnessFunction(length) {}

float OneMax::evaluate(Individual &ind) {
    int result = sum(ind.genotype);
    ind.fitness = result;
    
    checkIfBestFound(ind);
    
    return result;
}

void OneMax::display() {
    cout << "OneMax fitness function" << endl;
}

string OneMax::id() {
    return "om";
}

FitnessFunction* OneMax::clone() const {
    return new OneMax(static_cast<const OneMax&>(*this));
}


/* ------------------------ Leading Ones Fitness Function ------------------------ */

LeadingOnes::LeadingOnes(int length) : FitnessFunction(length) {}

float LeadingOnes::evaluate(Individual &ind) {
    float result = 0;
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

FitnessFunction* LeadingOnes::clone() const {
    return new LeadingOnes(static_cast<const LeadingOnes&>(*this));
}

void LeadingOnes::display() {
    cout << "LeadingOnes fitness function" << endl;
}

string LeadingOnes::id() {
    return "lo";
}


/* ------------------------ Trap Five Fitness Function ------------------------ */

TrapFive::TrapFive(int blocks) : FitnessFunction(blocks * 5), blocks(blocks), k(5) {}

float TrapFive::evaluate(Individual &ind) {
    float result = 0;
    for (int i = 0; i < blocks; i++) {
        result += subfunc(ind, i, i + k);
    }
    
    ind.fitness = result;
    checkIfBestFound(ind);
    
    return result;
}

float TrapFive::subfunc(Individual &ind, int startIdx, int endIdx) {
    float result = 0;
    for(int i = startIdx; i < endIdx; i++){
        result += ind.genotype[i];
    }
    
    if (result == k){
        return result;
    } else {
        return ((k - 1 - result) / k);
    }
}

FitnessFunction* TrapFive::clone() const {
    return new TrapFive(static_cast<const TrapFive&>(*this));
}

void TrapFive::display() {
    cout << "TrapFive fitness function" << endl;
}

string TrapFive::id() {
    return "t5";
}
