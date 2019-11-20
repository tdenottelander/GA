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

// int populationSize;
// FitnessFunction * fitFunc;
// Selection * selection;
// Variation * variation;
// vector<Individual> population;
// int problemLength;

GA::GA(int popSize, int probLength, FitnessFunction *f, Selection *s, Variation *v){
    populationSize = popSize;
    fitFunc = f;
    selection = s;
    variation = v;
    problemLength = probLength;
    
    initialize();
}

void GA::initialize(){
    population.reserve(populationSize);
    for(int i = 0; i < populationSize; i++){
        Individual ind (problemLength);
        ind.initialize();
        ind.fitness = fitFunc->evaluate(ind);
        population.push_back(ind);
    }
}

void GA::round(){
    // population = selection.select(population, population.size());
    population = variation->variate(population);
    evaluateAll(population);
    population = selection->select(population, population.size());
}

void GA::evaluateAll(vector<Individual> &population){
    for(Individual &ind: population){
        fitFunc->evaluate(ind);
//        ind.fitness = fitness;
    }
}

double GA::getAvgFitness(){
    double result = 0;
    for(Individual ind: population){
        result += ind.fitness;
    }
    return result / population.size();
}

string GA::toString() {
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

