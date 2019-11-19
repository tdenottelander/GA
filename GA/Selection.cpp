//
//  Selection.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "Selection.hpp"

using namespace std;
using namespace Utility;


/* ------------------------ Base Selection ------------------------ */

Selection::Selection(){}

vector<Individual> Selection::select(vector<Individual> &population, int outputSize) {
    cout << "Do a base selection" << endl;
    vector<Individual> v;
    return v;
}

void Selection::display(){
    cout << "Base selection" << endl;
}


/* ------------------------ Proportionate Selection ------------------------ */

ProportionateSelection::ProportionateSelection(){}

vector<Individual> ProportionateSelection::select(vector<Individual> &population, int outputSize) {
    vector<Individual> selection;
    selection.reserve(outputSize);
    vector<float> proportions = getProportions(population);
    for(int i = 0; i < outputSize; i++){
        float randomValue = getRand();
        for (int j = 0; j < proportions.size(); j++){
            if(randomValue < proportions[j]){
                selection.push_back(population[i]);
                break;
            }
        }
    }
    return selection;
}

void ProportionateSelection::display() {
    cout << "Proportionate selection" << endl;
}

vector<float> ProportionateSelection::getProportions(vector<Individual> &population){
    int n = (int)population.size();
    vector<float> proportions (n);
    int cumulativeFitness = 0;
    
    for(Individual ind: population){
        cumulativeFitness += ind.fitness;
    }
    
    for(int i = 0; i < n; i++){
        if(i == 0) {
            proportions[i] = (float)population[i].fitness / cumulativeFitness;
        } else {
            proportions[i] = proportions[i-1] + (float)population[i].fitness / cumulativeFitness;
        }
    }
    
    return proportions;
}


/* ------------------------ Tournament Selection ------------------------ */

//TODO: FINISH THIS
void TournamentSelection::select(vector<Individual> &population, int outputSize, int tournamentSize){
    int n = (int)population.size();
    vector<int> randomIndices;
    randomIndices.reserve(n);
    for (int i = 0; i < n; i++) randomIndices.push_back(i);
    shuffle(randomIndices.begin(), randomIndices.end(), default_random_engine(0));
    vector<Individual> newPopulation;
    newPopulation.reserve(n);
    
    for(int i = 0; i < (n/2); i++){
        int idx1 = randomIndices[i * 2];
        int idx2 = randomIndices[i * 2 + 1];
        
        if(population[idx1].fitness > population[idx2].fitness){
            newPopulation.push_back(population[idx1]);
        } else {
            newPopulation.push_back(population[idx2]);
        }
    }
}

void TournamentSelection::display() {
    cout << "Tournament selection" << endl;
}
