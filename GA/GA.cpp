//
//  GA.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "GA.hpp"

using namespace std;


/* ------------------------ Genetic Algorithm ------------------------ */

GA::GA() :
    terminated(false),
    initialized(false),
    roundsCount(0)
{}

void GA::initialize(){
//    initializeTrueRandomPopulation();
    initializeSolvablePopulation();
    
    initialized = true;
}

void GA::round(){
    roundPOVariationSelection();
    roundsCount++;
}

void GA::roundReplacementVariationSelection(){
    population = variation_ptr->variate(population);
    evaluateAll(population);
    population = selection_ptr->select(population, population.size());
}

void GA::roundPOVariationSelection(){
    vector<Individual> offspring = variation_ptr->variate(population);
    evaluateAll(offspring);
    
    vector<Individual> PO;
    PO.reserve(population.size() * 2);
    PO.insert(PO.begin(), population.begin(), population.end());
    PO.insert(PO.end(), offspring.begin(), offspring.end());
    
//    cout << "Combined P+O population: " << endl;
//    cout << populationToString(PO) << endl;

    population = selection_ptr->select(PO, population.size());
}

void GA::evaluateAll(vector<Individual> &population){
    for(Individual &ind: population){
        fitFunc_ptr->evaluate(ind);
    }
}

double GA::getAvgFitness(){
    double result = 0;
    for(Individual ind: population){
        result += ind.fitness;
    }
    return result / population.size();
}

bool GA::isConverged(){
    if(isOptimal() || !isDiverse()){
        terminated = true;
        return true;
    }
    return false;
}

bool GA::isDiverse(){
    for (int i = 1; i < population.size(); i++){
        if (population[i] != population[0]){
            return true;
        }
    }
    return false;
}

bool GA::isOptimal(){
    return fitFunc_ptr->optimumFound;
}

int GA::getTotalAmountOfEvaluations(){
    return fitFunc_ptr->evaluations;
}

void GA::initializeTrueRandomPopulation(){
    population.reserve(populationSize);
    for(int i = 0; i < populationSize; i++){
        Individual ind (problemLength);
        ind.initialize();
        fitFunc_ptr->evaluate(ind);
        population.push_back(ind);
    }
}

void GA::initializeSolvablePopulation(){
    population.reserve(populationSize);
    for(int i = 0; i < populationSize; i++){
        Individual ind (problemLength);
        population.push_back(ind);
    }
    
    for (int i = 0; i < problemLength; i++){
        vector<int> counters = {0, 0};
        for (Individual &ind : population){
            int bit = Utility::getConditionalBit(counters[0], counters[1], populationSize);
            ind.genotype[i] = bit;
            counters[bit] = counters[bit] + 1;
        }
    }
    
    evaluateAll(population);
}

string GA::toString() {
    return populationToString(population);
}

string GA::populationToString(vector<Individual> &population){
    string result;
    for (int i = 0; i < population.size(); i++){
        result += to_string(i);
        result += ". ";
        result += population[i].toString();
        if(i != population.size() - 1){
            result += "\n";
        }
    }
    return result;
}

void GA::setPopulationSize(int n){
    populationSize = n;
}

void GA::setProblemLength(int l){
    if(fitFunc_ptr != NULL){
        fitFunc_ptr->setLength(l);
    }
    problemLength = l;
}

void GA::setFitnessFunction (FitnessFunction * fitfunc){
    fitFunc_ptr = fitfunc;
}

void GA::setSelection (Selection * sel){
    selection_ptr = sel;
}

void GA::setVariation (Variation * var){
    variation_ptr = var;
}

void GA::setPointers(FitnessFunction * fitfunc, Selection * sel, Variation * var){
    setFitnessFunction(fitfunc);
    setSelection(sel);
    setVariation(var);
}

GA* GA::clone() const {
    return new GA(static_cast<const GA&>(*this));
}

string GA::id(){
    return "Base GA";
}

