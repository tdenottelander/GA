//
//  Variation.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "Variation.hpp"

using namespace std;
using namespace arma;
using namespace Utility;

/* ------------------------ Base Variation ------------------------ */

void Variation::display(){
    cout << "Base variation" << endl;
}

string Variation::id(){
    return "base";
}


/* ------------------------ Univariate Crossover Variation ------------------------ */

UnivariateCrossover::UnivariateCrossover(){}

vector<Individual> UnivariateCrossover::variate(vector<Individual> &population){
    int n = (int)population.size();
    vector<int> randomIndices = getRandomlyPermutedArrayV2(n);
    vector<Individual> newPopulation;
    newPopulation.reserve(n);
    
    for(int i = 0; i < (n/2); i++){
        int idx1 = randomIndices[i * 2];
        int idx2 = randomIndices[i * 2 + 1];
        
        pair<Individual, Individual> offspring = univariateCrossover(population[idx1], population[idx2]);
        
        newPopulation.push_back(offspring.first);
        newPopulation.push_back(offspring.second);
    }
    
    return newPopulation;
}

pair<Individual, Individual> UnivariateCrossover::univariateCrossover (Individual &ind1, Individual &ind2){
    Individual newInd1 = ind1.copy();
    Individual newInd2 = ind2.copy();
    
    for(int i = 0; i < ind1.genotype.size(); i++){
        if(getRand() < 0.5){
            newInd1.genotype[i] = ind2.genotype[i];
            newInd2.genotype[i] = ind1.genotype[i];
        }
    }
    
    pair<Individual, Individual> result(newInd1, newInd2);
    return result;
}

void UnivariateCrossover::display() {
    cout << "Univariate Crossover" << endl;
}

string UnivariateCrossover::id() {
    return "uni";
}


/* ------------------------ OnePoint Crossover Variation ------------------------ */

OnePointCrossover::OnePointCrossover(){}

vector<Individual> OnePointCrossover::variate(std::vector<Individual> &population){
    int n = (int)population.size();
    vector<int> randomIndices = getRandomlyPermutedArrayV2(n);
    vector<Individual> newPopulation;
    newPopulation.reserve(n);
    
    for(int i = 0; i < (n/2); i++){
        int idx1 = randomIndices[i * 2];
        int idx2 = randomIndices[i * 2 + 1];
        
//        cout << "Crossing over individual " << idx1 << " and " << idx2;
        pair<Individual, Individual> offspring = onePointCrossover(population[idx1], population[idx2]);
        
        newPopulation.push_back(offspring.first);
        newPopulation.push_back(offspring.second);
    }
    
    return newPopulation;
}

pair<Individual, Individual> OnePointCrossover::onePointCrossover(Individual &ind1, Individual &ind2){
    int genotypeLength = (int)ind1.genotype.size();
    int index = ceil(getRand() * genotypeLength);
//    cout << " on index " << index << endl;
    for (int i = index; i < genotypeLength; i++){
        int temp = (int)ind1.genotype[i];
        ind1.genotype[i] = ind2.genotype[i];
        ind2.genotype[i] = temp;
    }
    
    pair<Individual, Individual> result (ind1, ind2);
    return result;
}
    
void OnePointCrossover::display() {
    cout << "OnePoint Crossover" << endl;
}

string OnePointCrossover::id() {
    return "op";
}


/* ------------------------ GOM Variation ------------------------ */

GOM_Variation::GOM_Variation() {};

vector<Individual> GOM_Variation::variate(std::vector<Individual> &population){
    vector<Individual> offspring;
    offspring.reserve(population.size());
    for (Individual ind : population){
        Individual child = gom(ind, population);
        offspring.push_back(child);
    }
    return offspring;
}

Individual GOM_Variation::gom(Individual &ind, std::vector<Individual> &population){
    int popSize = population.size();
    Individual b = ind.copy();
    Individual o = ind.copy();
    
    for (uvec subset : fos) {
        Individual *p = &population[getRand(0, popSize)];
        applyDonor(o, *p, subset);
        
        if (o != b) {
            fitfunc->evaluate(o);
            if (o.fitness > b.fitness){
                applyDonor(b, o, subset);
                b.fitness = o.fitness;
            } else {
                applyDonor(o, b, subset);
                o.fitness = b.fitness;
            }
        }
    }
    return o;
}

void GOM_Variation::applyDonor(Individual &ind, Individual &parent, arma::uvec &subset){
    for (int idx : subset){
        ind.genotype[idx] = parent.genotype[idx];
    }
}

void GOM_Variation::setFOS(vector<arma::uvec> f){
    fos = f;
}

void GOM_Variation::display() {
    cout << "GOM Crossover" << endl;
}

string GOM_Variation::id() {
    return "gom";
}
