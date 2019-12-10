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

FitnessFunction::FitnessFunction(int optimum) : bestIndividual(NULL), optimum(optimum), optimumFound(false), evaluations(0) {
}

FitnessFunction::FitnessFunction() : bestIndividual(NULL), optimumFound(false), evaluations(0) {}

void FitnessFunction::display(){
    cout << "Base fitness function" << endl;
}

void FitnessFunction::checkIfBestFound(Individual &ind){
    if(bestIndividual == NULL || ind.fitness > bestIndividual.fitness){
        bestIndividual = ind.copy();
        if(ind.fitness == optimum){
            optimumFound = true;
        }
    }
}

string FitnessFunction::id() {
    return "base";
}

void FitnessFunction::setProblemType(ProblemType* problemType){
    this->problemType = problemType;
}

void FitnessFunction::setLength(int length){
    optimum = length;
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int length) : FitnessFunction(length) { setProblemType(); }
OneMax::OneMax() : FitnessFunction() { setProblemType(); }

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

void OneMax::setProblemType(){
    FitnessFunction::setProblemType(new BinaryProblemType());
}

FitnessFunction* OneMax::clone() const {
    FitnessFunction* result = new OneMax(static_cast<const OneMax&>(*this));
    result->problemType = this->problemType;
    return result;
}


/* ------------------------ Leading Ones Fitness Function ------------------------ */

LeadingOnes::LeadingOnes(int length) : FitnessFunction(length) { setProblemType(); }
LeadingOnes::LeadingOnes() : FitnessFunction() { setProblemType(); }

void LeadingOnes::setProblemType(){
    FitnessFunction::setProblemType(new BinaryProblemType());
}

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

TrapFive::TrapFive(int blocks) : FitnessFunction(blocks * 5), blocks(blocks), k(5) { setProblemType(); }
TrapFive::TrapFive() : FitnessFunction(), k(5) { setProblemType(); }

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

void TrapFive::setProblemType(){
    FitnessFunction::setProblemType(new BinaryProblemType());
}

void TrapFive::setLength (int length) {
    blocks = length;
    optimum = 5 * length;
}


/* ------------------------ Non-Binary Max Fitness Function ------------------------ */

NonBinaryMax::NonBinaryMax() {
    setProblemType();
}

//TODO: FINISH IMPLEMENTATION OF NONBINARY MAX FITNESS FUNCTION

float NonBinaryMax::evaluate(Individual &ind){
    float result = sum(ind.genotype);
    ind.fitness = result;
    return result;
}

FitnessFunction* NonBinaryMax::clone() const {
    return new NonBinaryMax(static_cast<const NonBinaryMax&>(*this));
}

void NonBinaryMax::display() {
    cout << "Non-binary Maximum fitness function" << endl;
}

string NonBinaryMax::id() {
    return "NBMax";
}

void NonBinaryMax::setProblemType() {
    vector<int> vec = {0, 1, 2, 3, 4, 5};
    FitnessFunction::setProblemType(new AlphabetProblemType(vec));
}

void NonBinaryMax::setLength(int length){
    optimum = length * 5;
}
