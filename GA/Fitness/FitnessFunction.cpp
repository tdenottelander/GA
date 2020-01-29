//
//  FitnessFunction.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "FitnessFunction.hpp"

using namespace std;
using namespace arma;

extern bool storeAbsoluteConvergence;
extern bool storeUniqueConvergence;
extern bool storeTransformedUniqueConvergence;
extern nlohmann::json convergence;

/* ------------------------ Base Fitness Function ------------------------ */

FitnessFunction::FitnessFunction(float optimum, ProblemType *problemType) :
    bestIndividual(0),
    optimum(optimum),
    optimumFound(false),
    maxEvaluations(-1),
    maxUniqueEvaluations(-1),
    problemType(problemType),
    totalEvaluations(0),
    totalUniqueEvaluations(0),
    totalTransformedUniqueEvaluations(0),
    uniqueSolutions(problemType->alphabet.size()),
    transformedUniqueSolutions(problemType->alphabet.size())
{
}

FitnessFunction::FitnessFunction(ProblemType *problemType) :
    bestIndividual(0),
    optimumFound(false),
    maxEvaluations(-1),
    maxUniqueEvaluations(-1),
    problemType(problemType),
    totalEvaluations(0),
    totalUniqueEvaluations(0),
    totalTransformedUniqueEvaluations(0),
    uniqueSolutions(problemType->alphabet.size()),
    transformedUniqueSolutions(problemType->alphabet.size())
{
}

void FitnessFunction::clear(){
    bestIndividual = 0;
    optimumFound = false;
    totalEvaluations = 0;
    totalUniqueEvaluations = 0;
    totalTransformedUniqueEvaluations = 0;
    uniqueSolutions = UniqueSolutions(problemType->alphabet.size());
    transformedUniqueSolutions = UniqueSolutions(problemType->alphabet.size());
}

// Performs additional operations like incrementing the amount of (unique) evaluations, checking whether an individual is the best so far yet and storing convergence data.
void FitnessFunction::evaluationProcedure(Individual &ind){
    checkIfBestFound(ind);
    totalEvaluations++;
//    float roundedFitness = roundf(bestIndividual.fitness * 10) / 10;  // Rounds to 3 decimal places
    float roundedFitness = bestIndividual.fitness;
    
    if(storeAbsoluteConvergence){
        convergence["absolute"].push_back(roundedFitness);
    }
    
    if(storeUniqueConvergence && !uniqueSolutions.contains(ind.genotype)){
        uniqueSolutions.put(ind.genotype);
        totalUniqueEvaluations++;
        convergence["unique"].push_back(roundedFitness);
    }
    
    if(storeTransformedUniqueConvergence){
        uvec transformedGenotype = transform(ind.genotype);
        if (!transformedUniqueSolutions.contains(transformedGenotype)){
            transformedUniqueSolutions.put(transformedGenotype);
            totalTransformedUniqueEvaluations++;
            convergence["transformedUnique"].push_back(roundedFitness);
        }
    }
}

// Displays the description of the fitness function
void FitnessFunction::display(){
    cout << "Base fitness function" << endl;
}

// Checks whether this individual is fitter than the best found individual so far.
// Checks whether the individual is optimal.
void FitnessFunction::checkIfBestFound(Individual &ind){
    
//    if(checkForGenotype && ind.genotypeEquals(optimalGenotype)){
//        ind.fitness += 0.01;
//    }
    
    if(ind.fitness > bestIndividual.fitness){
        bestIndividual = ind.copy();
//        cout << "this genotype: " << Utility::genotypeToString(ind.genotype) << "  opt genotype: " << Utility::genotypeToString(optimalGenotype) << endl;
        if((!checkForGenotype && ind.fitness >= optimum) || (checkForGenotype && ind.genotypeEquals(optimalGenotype))){
            optimumFound = true;
        }
    }
}

void FitnessFunction::setGenotypeChecking(uvec genotype){
    checkForGenotype = true;
    optimalGenotype = genotype;
}

// Returns the total amount of evaluations over all fitness functions.
int FitnessFunction::getTotalAmountOfEvaluations(){
    return totalEvaluations;
}

// Checks whether the maximum amount of evaluations is exceeded
bool FitnessFunction::maxEvaluationsExceeded(){
    return totalEvaluations >= maxEvaluations && maxEvaluations != -1;
}

bool FitnessFunction::maxUniqueEvaluationsExceeded(){
    return totalUniqueEvaluations >= maxUniqueEvaluations && maxUniqueEvaluations != -1;
}

// Returns the id of the fitness function
string FitnessFunction::id() {
    return "base";
}

// Sets the length and the optimum
void FitnessFunction::setLength(int length){
    totalProblemLength = length;
    optimum = length;
}

// Transforms the genotype (e.g. remove identity layers in ARK)
// Should be overridden in derived classes
uvec FitnessFunction::transform(uvec &genotype){
    return genotype;
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int length) : FitnessFunction(length, getProblemType()) {}
OneMax::OneMax() : FitnessFunction(getProblemType()) {}

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

ProblemType* OneMax::getProblemType(){
    return new BinaryProblemType();
}

FitnessFunction* OneMax::clone() const {
    FitnessFunction* result = new OneMax(static_cast<const OneMax&>(*this));
    result->problemType = this->problemType;
    return result;
}


/* ------------------------ Leading Ones Fitness Function ------------------------ */

LeadingOnes::LeadingOnes(int length) : FitnessFunction(length, getProblemType()) {}
LeadingOnes::LeadingOnes() : FitnessFunction(getProblemType()) {}

ProblemType* LeadingOnes::getProblemType(){
    return new BinaryProblemType();
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

NonBinaryMax::NonBinaryMax() : FitnessFunction(getProblemType()) {}

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

ProblemType* NonBinaryMax::getProblemType(){
    vector<int> vec = {0, 1, 2, 3, 4, 5};
    return new AlphabetProblemType(vec);
}

void NonBinaryMax::setLength(int length){
    totalProblemLength = length * 5;
    optimum = length * 5;
}
