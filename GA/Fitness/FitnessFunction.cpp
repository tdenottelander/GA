//
//  FitnessFunction.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "FitnessFunction.hpp"

using namespace std;

extern int totalEvaluations;
extern int totalUniqueEvaluations;
extern nlohmann::json convergence;
extern UniqueSolutions uniqueSolutions;

/* ------------------------ Base Fitness Function ------------------------ */

FitnessFunction::FitnessFunction(float optimum, int maxEvaluations) : bestIndividual(0), optimum(optimum), optimumFound(false), evaluations(0), maxEvaluations(maxEvaluations) {
}

FitnessFunction::FitnessFunction(int maxEvaluations) : bestIndividual(0), optimumFound(false), evaluations(0), maxEvaluations(maxEvaluations) {
}

// Performs additional operations like incrementing the amount of (unique) evaluations, checking whether an individual is the best so far yet and storing convergence data.
void FitnessFunction::evaluationProcedure(Individual &ind){
    checkIfBestFound(ind);
    evaluations++;
    totalEvaluations++;
    convergence["absolute"].push_back(bestIndividual.fitness);
//    convergence["absolute"][to_string(totalEvaluations)] = bestIndividual.fitness;
    
    if(!uniqueSolutions.contains(ind.genotype)){
        uniqueSolutions.put(ind.genotype);
        totalUniqueEvaluations++;
        convergence["unique"].push_back(bestIndividual.fitness);
//        convergence["unique"][to_string(totalUniqueEvaluations)] = bestIndividual.fitness;
    }
}

// Displays the description of the fitness function
void FitnessFunction::display(){
    cout << "Base fitness function" << endl;
}

// Checks whether this individual is fitter than the best found individual so far.
// Checks whether the individual is optimal.
void FitnessFunction::checkIfBestFound(Individual &ind){
    if(ind.fitness > bestIndividual.fitness){
        bestIndividual = ind.copy();
        if(ind.fitness >= optimum){
            optimumFound = true;
        }
    }
}

// Returns the total amount of evaluations over all fitness functions.
int FitnessFunction::getTotalAmountOfEvaluations(){
    return totalEvaluations;
}

// Checks whether the maximum amount of evaluations is exceeded
bool FitnessFunction::maxEvaluationsExceeded(){
    return totalEvaluations >= maxEvaluations && maxEvaluations != -1;
}

// Returns the id of the fitness function
string FitnessFunction::id() {
    return "base";
}

// Sets the problem type
void FitnessFunction::setProblemType(ProblemType* problemType){
    this->problemType = problemType;
}

// Sets the length and the optimum
void FitnessFunction::setLength(int length){
    totalProblemLength = length;
    optimum = length;
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int length, int maxEvaluations) : FitnessFunction(length, maxEvaluations) { setProblemType(); }
OneMax::OneMax(int maxEvaluations) : FitnessFunction(maxEvaluations) { setProblemType(); }

float OneMax::evaluate(Individual &ind) {
    int result = sum(ind.genotype);
    ind.fitness = result;
    
    evaluationProcedure(ind);
    
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

LeadingOnes::LeadingOnes(int length, int maxEvaluations) : FitnessFunction(length, maxEvaluations) { setProblemType(); }
LeadingOnes::LeadingOnes(int maxEvaluations) : FitnessFunction(maxEvaluations) { setProblemType(); }

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
    
    evaluationProcedure(ind);
    
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


/* ------------------------ Non-Binary Max Fitness Function ------------------------ */

NonBinaryMax::NonBinaryMax(int maxEvaluations) : FitnessFunction(maxEvaluations) {
    setProblemType();
}

//TODO: FINISH IMPLEMENTATION OF NONBINARY MAX FITNESS FUNCTION

float NonBinaryMax::evaluate(Individual &ind){
    float result = sum(ind.genotype);
    ind.fitness = result;
    evaluationProcedure(ind);
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
    totalProblemLength = length * 5;
    optimum = length * 5;
}
