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

extern bool printfos;

/* ------------------------ Base Variation ------------------------ */

vector<Individual> Variation::variate(vector<Individual> &population){
    int n = (int)population.size();
    vector<int> randomIndices = getRandomlyPermutedArrayV2(n);
    vector<Individual> newPopulation;
    newPopulation.reserve(n);
    
    for(int i = 0; i < (n/2); i++){
        int idx1 = randomIndices[i * 2];
        int idx2 = randomIndices[i * 2 + 1];
        
        pair<Individual, Individual> offspring = crossover(population[idx1], population[idx2]);
        
        newPopulation.push_back(offspring.first);
        newPopulation.push_back(offspring.second);
    }
    
    return newPopulation;
}

std::pair<Individual, Individual> Variation::crossover(Individual &ind1, Individual &ind2){
    cout << "Not implemented. Returning same individuals" << endl;
    pair<Individual, Individual> pair (ind1, ind2);
    return pair;
}

void Variation::display(){
    cout << "Base variation" << endl;
}

string Variation::id(){
    return "base";
}


/* ------------------------ Univariate Crossover Variation ------------------------ */

UnivariateCrossover::UnivariateCrossover(){}

pair<Individual, Individual> UnivariateCrossover::crossover (Individual &ind1, Individual &ind2){
    Individual newInd1 = ind1.copy();
    Individual newInd2 = ind2.copy();
    
    for(unsigned long i = 0; i < ind1.genotype.size(); i++){
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

pair<Individual, Individual> OnePointCrossover::crossover(Individual &ind1, Individual &ind2){
    Individual newInd1 = ind1.copy();
    Individual newInd2 = ind2.copy();
    
    int beginIndex = ceil(getRand() * ind1.genotype.size() - 2);
    for(int i = beginIndex; i < ind1.genotype.size(); i++){
        newInd1.genotype[i] = ind2.genotype[i];
        newInd2.genotype[i] = ind1.genotype[i];
    }
    
    pair<Individual, Individual> result(newInd1, newInd2);
    return result;
}
    
void OnePointCrossover::display() {
    cout << "OnePoint Crossover" << endl;
}

string OnePointCrossover::id() {
    return "op";
}


/* ------------------------ OnePoint Crossover Variation ------------------------ */

TwoPointCrossover::TwoPointCrossover(){}

pair<Individual, Individual> TwoPointCrossover::crossover(Individual &ind1, Individual &ind2){
    Individual newInd1 = ind1.copy();
    Individual newInd2 = ind2.copy();
    
    // It is possible that the indices are the same. In that case, no crossover happens.
    // It is also possible for either index to be 0 or size()-1. In that case, it becomes one-point crossover.
    int firstIndex = floor(getRand() * ind1.genotype.size());
    int secondIndex = floor(getRand() * ind2.genotype.size());
    for(int i = min(firstIndex, secondIndex); i < max(firstIndex, secondIndex); i++){
        newInd1.genotype[i] = ind2.genotype[i];
        newInd2.genotype[i] = ind1.genotype[i];
    }
    
    pair<Individual, Individual> result(newInd1, newInd2);
    return result;
}

void TwoPointCrossover::display() {
    cout << "TwoPoint Crossover" << endl;
}

string TwoPointCrossover::id() {
    return "tp";
}



/* ------------------------ GOM Variation ------------------------ */

GOM_Variation::GOM_Variation(bool forcedImprovement) : forcedImprovement(forcedImprovement) {
};

vector<Individual> GOM_Variation::variate(std::vector<Individual> &population){
    fos = fosObject->getFOS(population);
//    FOSStructures::printFOS(fos);
    vector<Individual> offspring;
    if(printfos){ FOSStructures::printFOS(fos); }
    offspring.reserve(population.size());
    for (unsigned i = 0; i < population.size(); i++){
        Individual &parent = population[i];
        Individual child = gom(parent, population, i);
        if(child.fitness > parent.fitness){
            child.counterNotChanged = 0;
        } else {
            child.counterNotChanged++;
        }
        offspring.push_back(child);
        if(fitfunc->optimumFound || fitfunc->maxEvaluationsExceeded() || fitfunc->maxUniqueEvaluationsExceeded()){
            break;
        }
    }
    return offspring;
}

Individual GOM_Variation::gom(Individual &ind, std::vector<Individual> &population, int indIdx){
    int popSize = population.size();
    Individual b = ind.copy();
    Individual o = ind.copy();
    bool changed = false;
    
//    cout << "start\n";
    for (uvec subset : fos) {
//        for (int i : subset) cout << i << " ";
//        cout << endl;
        
        // Find a donor that is different than this individual.
        int donorIdx = indIdx;
        while(donorIdx == indIdx){
            donorIdx = getRand(0, popSize);
        }
        Individual *p = &population[donorIdx];
        applyDonor(o, *p, subset);
        
        if (!o.equals(b)) {
            fitfunc->evaluate(o);
            if (o.fitness >= b.fitness){
                applyDonor(b, o, subset);
                b.fitness = o.fitness;
                changed = true;
            } else {
                applyDonor(o, b, subset);
                o.fitness = b.fitness;
            }
        }
        if(fitfunc->optimumFound || fitfunc->maxEvaluationsExceeded() || fitfunc->maxUniqueEvaluationsExceeded()){
            break;
        }
    }
//    cout << "end\n";
//    cout << endl;
    
    if(!fitfunc->optimumFound && !fitfunc->maxEvaluationsExceeded() && forcedImprovement && (!changed || o.counterNotChanged > (1 + log(popSize) / log(10)))){
        changed = gomWithEliteIndividual(o, b);
        
        if(!changed){
            o = fitfunc->bestIndividual.copy();
        }
    }
    
    return o;
}

bool GOM_Variation::gomWithEliteIndividual(Individual &o, Individual &b){
    bool changed = false;
    for (uvec subset : fos) {
        applyDonor(o, fitfunc->bestIndividual, subset);
        if(!o.equals(b)) {
            fitfunc->evaluate(o);
            if(o.fitness > b.fitness) {
                applyDonor(b, o, subset);
                b.fitness = o.fitness;
                changed = true;
            } else {
                applyDonor(o, b, subset);
                o.fitness = b.fitness;
            }
        }
        if(changed){
            break;
        }
    }
    return changed;
}

void GOM_Variation::applyDonor(Individual &ind, Individual &parent, arma::uvec &subset){
    for (int idx : subset){
        ind.genotype[idx] = parent.genotype[idx];
    }
}

void GOM_Variation::setFOSObject(FOS *f){
    fosObject = f;
}

void GOM_Variation::display() {
    cout << "GOM Crossover" << endl;
}

string GOM_Variation::id() {
    return "gom";
}
