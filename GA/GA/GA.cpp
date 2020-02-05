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

GA::GA(FitnessFunction * fitfunc) :
    terminated(false),
    initialized(false),
    roundsCount(0),
    fitFunc_ptr(fitfunc)
{}

void GA::initialize(){
    initializeTrueRandomPopulation();
//    initializeSolvablePopulation();
    terminated = false;
    roundsCount = 0;
    initialized = true;
}

void GA::run(){
    while(!isConverged()){
        round();
    }
}

void GA::round(){
    roundPOVariationSelection();
    roundsCount++;
}

int GA::findMinimallyNeededPopulationSize(int repetitions, int successesNeeded){
    GA* ga = clone();
    int left = -1;
    int right = 2;
    int populationSize = right;
    bool doublingPhase = true;
    while(populationSize != left){
        int fails = 0;
        for (int i = 0; i < repetitions; i++){
            ga->fitFunc_ptr->clear();
            ga->setPopulationSize(populationSize);
            ga->initialize();
            ga->evaluateAll();
            
            ga->run();
            
            if(!ga->isOptimal()){
//                cout << "x";
                fails += 1;
                if(fails > repetitions - successesNeeded){
                    break;
                }
            } else {
//                cout << ".";
            }
        }
        
//        cout << endl;
        
        if(fails > repetitions - successesNeeded){
            if(doublingPhase){
                left = populationSize;
                populationSize *= 2;
                right = populationSize;
//                cout << "fail(d), now l=" << left << " p=" << populationSize << " r=" << right << endl;
            } else {
                left = populationSize;
                populationSize = left + floor((right - left) / 2);
//                cout << "fail,    now l=" << left << " p=" << populationSize << " r=" << right << endl;
            }
        } else {
            doublingPhase = false;
            right = populationSize;
            populationSize = left + floor((right - left) / 2);
//            cout << "success, now l=" << left << " p=" << populationSize << " r=" << right << endl;
        }
    }
    
//    cout << "                                              ";
    cout << "PROBLEMSIZE = " << fitFunc_ptr->totalProblemLength << "   NEEDED POPULATION SIZE = " << populationSize + 1 << endl;
//    cout << endl;
    ga->fitFunc_ptr->clear();
    return populationSize + 1;
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
        if(fitFunc_ptr->optimumFound || fitFunc_ptr->maxEvaluationsExceeded() || fitFunc_ptr->maxUniqueEvaluationsExceeded()){
            break;
        }
    }
}

void GA::evaluateAll(){
    evaluateAll(population);
}

vector<float> GA::getAvgFitness(){
    vector<float> result (population[0].fitness.size(), 0);
    for(Individual ind: population){
        for (int obj = 0; obj < ind.fitness.size(); obj++){
            result[obj] += ind.fitness[obj];
        }
    }
    for (int obj = 0; obj < population[0].fitness.size(); obj++){
        result[obj] = result[obj] / population.size();
    }
    return result;
}

bool GA::isConverged(){
    if(isOptimal() || !isDiverse()){
        terminated = true;
        return true;
    }
    return false;
}

bool GA::isDiverse(){
    for (unsigned long i = 1; i < population.size(); i++){
        if (!population[i].equals(population[0])){
            return true;
        }
    }
    return false;
}

bool GA::isOptimal(){
    return fitFunc_ptr->optimumFound;
}

int GA::getTotalAmountOfEvaluations(){
    return fitFunc_ptr->totalEvaluations;
}

void GA::initializeTrueRandomPopulation(){
    population = vector<Individual>();
    population.reserve(populationSize);
    vector<int> alphabet = fitFunc_ptr->problemType->alphabet;
    for(int i = 0; i < populationSize; i++){
        Individual ind (fitFunc_ptr->totalProblemLength, fitFunc_ptr->numObjectives);
        ind.initialize(alphabet);
        population.push_back(ind);
    }
}

//TODO: Tweak this such that it works on non-binary GA's
void GA::initializeSolvablePopulation(){
    initializeUninitializedPopulation();
    
    for (int i = 0; i < fitFunc_ptr->totalProblemLength; i++){
        vector<int> counters = {0, 0};
        for (Individual &ind : population){
            int bit = Utility::getConditionalBit(counters[0], counters[1], populationSize);
            ind.genotype[i] = bit;
            counters[bit] = counters[bit] + 1;
        }
    }
}

void GA::initializeUninitializedPopulation(){
    population = vector<Individual>();
    population.reserve(populationSize);
    for(int i = 0; i < populationSize; i++){
        Individual ind (fitFunc_ptr->totalProblemLength, fitFunc_ptr->numObjectives);
        population.push_back(ind);
    }
}

string GA::toString() {
    return populationToString(population);
}

string GA::populationToString(vector<Individual> &population){
    string result;
    string fitfuncid = fitFunc_ptr->id();
    bool trapfunc = fitfuncid.at(0) == 'T';
    for (unsigned long i = 0; i < population.size(); i++){
        result += to_string(i);
        result += ". ";
        if(trapfunc){
            result += population[i].toStringBlocks(stoi(fitfuncid.substr(1, fitfuncid.length())));
        } else {
            result += population[i].toString();
        }
        if(i != population.size() - 1){
            result += "\n";
        }
    }
    return result;
}

void GA::print(){
    cout << toString() << endl;
}

void GA::setPopulationSize(int n){
    populationSize = n;
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

void GA::printPopulationGenotypeStatistics(){
    vector<vector<int>> counts;
    for (int i = 0; i < fitFunc_ptr->problemType->alphabet.size(); i++){
        vector<int> subvec (fitFunc_ptr->totalProblemLength, 0);
        counts.push_back(subvec);
    }
    
    for (int i = 0; i < population.size(); i++){
        Individual *ind = &population[i];
        for (int j = 0; j < fitFunc_ptr->totalProblemLength; j++){
            counts[ind->genotype[j]][j] += 1;
        }
    }
    
    string result = "";
    for (int i = 0; i < counts.size(); i++){
        result += to_string(fitFunc_ptr->problemType->alphabet[i]);
        result += ":  ";
        for (int j = 0; j < counts[i].size(); j++){
            result += Utility::padWithSpacesAfter(to_string(counts[i][j]), 5);
            if (j < counts[i].size() - 1)
                result += "|";
        }
        result += "\n";
    }
    
    cout << result << endl;
}
