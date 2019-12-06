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

FitnessFunction::FitnessFunction(int optimum) : bestIndividual(0), optimum(optimum), optimumFound(false), evaluations(0) {
}

FitnessFunction::FitnessFunction(ProblemType* problemType) : bestIndividual(0), optimumFound(false), evaluations(0), problemType(problemType) {}

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

void FitnessFunction::setLength(int length){
    optimum = length;
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int length) : FitnessFunction(length) {}
OneMax::OneMax(ProblemType* problemType) : FitnessFunction(problemType) {}

float OneMax::evaluate(Individual &ind) {
    int result = sum(ind.genotype);
    ind.fitness = result;
    
    checkIfBestFound(ind);
    
    evaluations++;
    return result;
}

void OneMax::display() {
    cout << "OneMax fitness function" << endl;
}

string OneMax::id() {
    return "OM";
}

FitnessFunction* OneMax::clone() const {
    FitnessFunction* result = new OneMax(static_cast<const OneMax&>(*this));
    result->problemType = this->problemType;
    return result;
}


/* ------------------------ Leading Ones Fitness Function ------------------------ */

LeadingOnes::LeadingOnes(int length) : FitnessFunction(length) {}
LeadingOnes::LeadingOnes(ProblemType* problemType) : FitnessFunction(problemType) {}

float LeadingOnes::evaluate(Individual &ind) {
    float result = 0;
    for (unsigned long i = 0; i < ind.genotype.size(); i++){
        if (ind.genotype[i] == 0){
            break;
        } else {
            result++;
        }
    }
    ind.fitness = result;
    
    
    
    checkIfBestFound(ind);
    
    evaluations++;
    return result;
}

FitnessFunction* LeadingOnes::clone() const {
    FitnessFunction* result = new LeadingOnes(static_cast<const LeadingOnes&>(*this));
    result->problemType = this->problemType;
    return result;
}

void LeadingOnes::display() {
    cout << "LeadingOnes fitness function" << endl;
}

string LeadingOnes::id() {
    return "LO";
}


/* ------------------------ Trap Five Fitness Function ------------------------ */

TrapFive::TrapFive(int blocks) : FitnessFunction(blocks * 5), blocks(blocks), k(5) {}
TrapFive::TrapFive(ProblemType* problemType) : FitnessFunction(problemType), k(5) {}

float TrapFive::evaluate(Individual &ind) {
    float result = 0;
    for (int i = 0; i < blocks; i++) {
        result += subfunc(ind, i, i + k);
    }
    
    ind.fitness = result;
    checkIfBestFound(ind);
    
    evaluations++;
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
    return "T5";
}

void TrapFive::setLength (int length) {
    blocks = length;
    optimum = 5 * length;
}
