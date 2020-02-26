//
//  NSGA_II.cpp
//  GA
//
//  Created by Tom den Ottelander on 03/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "NSGA_II.hpp"

using namespace std;

NSGA_II::NSGA_II(FitnessFunction * fitFunc) : NSGA_II::NSGA_II(fitFunc, new TwoPointCrossover(), 2, 0.9, true, false){}

NSGA_II::NSGA_II(FitnessFunction * fitFunc, Variation * var, int tournamentSize, float crossoverProbability, bool mutation, bool visualize) : SimpleGA(fitFunc, var, NULL), tournamentSize(tournamentSize), crossoverProbability(crossoverProbability), doMutation(mutation), visualize(visualize) {
}

void NSGA_II::round() {
    // Do an initial non dominating sorting (rank assigning) and crowding distance assigning at the start of the first round
    if(roundsCount == 0){
        sortedPopulation = nonDominatedSorting(population);
        fitFunc_ptr->updateElitistArchive(sortedPopulation[0]);
        parentPop = truncate(sortedPopulation);
        if(visualize) draw2DVisualization(population, fitFunc_ptr->optimum[0]+1, fitFunc_ptr->optimum[1]+1);
    }
    
    // Do tournament selection on the truncated parent population to create a child population
    vector<Individual> childPop = selection(parentPop);
    
    // Mutate the child population and evaluate all resulting individuals
    mutation(childPop);
    evaluateAll(childPop);
    
    // Merge Pt and Qt
    vector<Individual> mergedPopulation = merge(parentPop, childPop);

    // Sort the merged population on rank
    sortedPopulation = nonDominatedSorting(mergedPopulation);
    
    // Select Pt by truncating based on rank and crowding distance
    parentPop = truncate(sortedPopulation);
    population = parentPop;
    
    if(visualize) draw2DVisualization(population, fitFunc_ptr->optimum[0]+1, fitFunc_ptr->optimum[1]+1);
    
    // Update elitist archive with the best front of the sorted population
    bool updated = fitFunc_ptr->updateElitistArchive(sortedPopulation[0]);
    if (updated){
        noAdditionToElitistArchiveCount = 0;
    } else {
        noAdditionToElitistArchiveCount++;
        if (noAdditionToElitistArchiveCount >= 10){
//            converged = true;
        }
    }
    
    roundsCount++;
}

void NSGA_II::clearMOinformation(vector<Individual> &population){
    for (int i = 0; i < population.size(); i++){
        population[i].clearMOinformation();
    }
}

// Sorts the individuals into fronts. If n is not -1, then there process of sorting into fronts stops when
// finding all the solutions of a front is done and the total amount of solutions is at least n.
vector<vector<Individual*>> NSGA_II::nonDominatedSorting (vector<Individual> &population, int n){
    
    // Clear the information like front and crowding distance from the population.
    clearMOinformation(population);
    // Loop over every individual combination to see which individuals dominate which other individuals.
    for (int i = 0; i < population.size(); i++){
        for (int j = 0; j < population.size(); j++){
            if (population[i].dominates(population[j])){
                // Individual i dominates individual j
                population[j].dominationCount += 1;
                population[i].dominationList.push_back(&population[j]);
            }
        }
    }
    
    // Put a pointer to every individual in a pool that we can iterate over and modify.
    list<Individual*> pool;
    for (int i = 0; i < population.size(); i++){
        pool.push_back(&population[i]);
    }
    
    sortedPopulation.clear();
    int frontIdx = 0;
    int individualCount = 0;
    // See which individuals have domination count 0. Put these in the same front.
    while (!pool.empty()) {
        vector<Individual*> front;
        for (list<Individual*>::iterator it = pool.begin(); it != pool.end(); it++){
            if ((*it)->dominationCount == 0){
                (*it)->front = frontIdx;
                front.push_back(*it);
                pool.erase(it);
                individualCount++;
            }
        }
        
        // Modify domination count of other individuals by subtracting the dominates of dominated solutions by solutions from this front.
        for (Individual* can : front){
            for (Individual *dominatingIndividual : can->dominationList){
                dominatingIndividual->dominationCount -= 1;
            }
        }
        
        sortedPopulation.push_back(front);
        frontIdx++;
        // If we only need [n] individuals, we can stop assigning fronts.
        if(n != -1 && individualCount >= n){
            break;
        }
    }
    return sortedPopulation;
}

// Sorts the individuals in a front based on the crowding distance.
void NSGA_II::CrowdingDistanceSorting (vector<Individual*> &front){
    for (int obj = 0; obj < fitFunc_ptr->numObjectives; obj++){
//        cout << "UNSORTED FRONT: " << endl;
//        for (int i = 0; i < front.size(); i++){
//            cout << i << ": " << front[i]->toString() << endl;
//        }

        // Sort individuals in this front on fitness for objective [obj].
        quickSort(front, obj);
        
        front.front()->crowdingDistance = INFINITY;
        front.back()->crowdingDistance = INFINITY;
        
        if (front.size() > 2){
            float maxObjFitness = front.back()->fitness[obj];
            float minObjFitness = front.front()->fitness[obj];
            float normalizationValue = maxObjFitness - minObjFitness;
            
            for (int i = 1; i < front.size() - 1; i++){
                if(front[i]->crowdingDistance != INFINITY){
                    float prevObjFitness = front[i-1]->fitness[obj];
                    float nextObjFitness = front[i+1]->fitness[obj];
                    float newCrowdingDistance;
                    if (normalizationValue == 0){
                        newCrowdingDistance = INFINITY;
                    } else {
                        newCrowdingDistance = front[i]->crowdingDistance + ((nextObjFitness - prevObjFitness) / normalizationValue);
                    }
                    front[i]->crowdingDistance = newCrowdingDistance;
                }
            }
        }
        
//        cout << "SORTED FRONT: " << endl;
//        for (int i = 0; i < front.size(); i++){
//            cout << i << ": " << front[i]->toString() << endl;
//        }
    }
    
    sort(front.begin(), front.end(), [](const Individual* rhs, const Individual* lhs){
        return lhs->crowdingDistance < rhs->crowdingDistance;
    });
}

void NSGA_II::insertionSort(vector<Individual*> &front, int objectiveIdx){
    for (int i = 0; i < front.size(); i++){
        int moveIdx = i;
        while (moveIdx != 0){
            if (front[moveIdx]->fitness[objectiveIdx] <= front[moveIdx-1]->fitness[objectiveIdx]){
                Individual* temp = front[moveIdx-1];
                front[moveIdx-1] = front[moveIdx];
                front[moveIdx] = temp;
                moveIdx--;
            } else {
                break;
            }
        }
    }
}

void NSGA_II::quickSort(vector<Individual*> &front, int objectiveIdx){
    quickSort(front, objectiveIdx, 0, front.size() - 1);
}

void NSGA_II::quickSort(vector<Individual*> &front, int objectiveIdx, int left, int right){
    if (left < right){
        int index = Utility::getRand(left, right);
        Individual* temp = front[right];
        front[right] = front[index];
        front[index] = temp;
        float pivot = front[right]->fitness[objectiveIdx];
        int i = left - 1;
        for (int j = left; j < right; j++){
            if (front[j]->fitness[objectiveIdx] <= pivot){
                i += 1;
                temp = front[j];
                front[j] = front[i];
                front[i] = temp;
            }
        }
        index = i + 1;
        temp = front[index];
        front[index] = front[right];
        front[right] = temp;
        quickSort(front, objectiveIdx, left, index - 1);
        quickSort(front, objectiveIdx, index + 1, right);
    }
}

// Returns true if can1 is favoured over can2
bool NSGA_II::crowdComparisonOperator(const Individual &lhs, const Individual &rhs){
    if (lhs.front < rhs.front){
        return true;
    } else if (lhs.front > rhs.front){
        return false;
    } else if (lhs.crowdingDistance > rhs.crowdingDistance){
        return true;
    } else if (lhs.crowdingDistance < rhs.crowdingDistance){
        return false;
    } else if (Utility::getRand() < 0.5) {
        return true;
    } else {
        return false;
    }
}

Individual NSGA_II::tournament(Individual &ind1, Individual &ind2){
    if (crowdComparisonOperator(ind1, ind2)){
        return ind1;
    } else {
        return ind2;
    }
}


vector<Individual> NSGA_II::truncate(vector<vector<Individual*>> sortedPopulation){
    vector<Individual> parentPop;
    parentPop.reserve(populationSize);
    int individualsAdded = 0;
    
    for (vector<Individual*> &front : sortedPopulation){
        CrowdingDistanceSorting(front);
        for (int i = 0; i < front.size(); i++){
            front[i]->canReproduce = true;
            parentPop.push_back(front[i]->copy());
//            cout << individualsAdded << ":  front=" << front[i]->front << "  crDist=" << front[i]->crowdingDistance << endl;
            if (++individualsAdded == populationSize){
                return parentPop;
            }
        }
    }
    return parentPop;
}

vector<Individual> NSGA_II::selection(vector<Individual> parentPop){
    vector<Individual> childPop;
    childPop.reserve(populationSize);

    if (populationSize % 4 != 0){
        cout << "Consider setting the population size to a multiple of 4.";
    }
    
    int index = 0;
    vector<int> randIdxArray = Utility::getOrderedArray(populationSize, Utility::Order::RANDOM);
    for (int i = 0; i < populationSize; i+=2){
        if (index+3 >= populationSize){
            randIdxArray = Utility::getOrderedArray(populationSize, Utility::Order::RANDOM);
        }

        Individual parent1 = tournament(parentPop[randIdxArray[index]], parentPop[randIdxArray[index+1]]);
        Individual parent2 = tournament(parentPop[randIdxArray[index+2]], parentPop[randIdxArray[index+3]]);
        pair<Individual, Individual> offspring = variation_ptr->crossover(parent1, parent2);
        childPop.push_back(offspring.first);
        
        if (i+1 < populationSize){
            childPop.push_back(offspring.second);
        }
    }

    return childPop;
}

void NSGA_II::mutation(vector<Individual> &children){
    float mutationFactor = 1.0f / fitFunc_ptr->totalProblemLength;
    for (int i = 0; i < children.size(); i++){
        mutate(children[i], mutationFactor);
    }
}

void NSGA_II::mutate(Individual &ind, float probability){
    for (int gene = 0; gene < ind.genotype.size(); gene++){
        float rand = Utility::getRand();
        if (rand < probability) {
            if(ind.genotype[gene] == 0){
                ind.genotype[gene] = 1;
            } else {
                ind.genotype[gene] = 0;
            }
        }
    }
}

vector<Individual> NSGA_II::merge(vector<Individual> &parentPop, vector<Individual> &childPop){
    vector<Individual> mergedPop;
    for (int i = 0; i < parentPop.size(); i++){
        mergedPop.push_back(parentPop[i].copy());
    }
    for (int i = 0; i < childPop.size(); i++){
        mergedPop.push_back(childPop[i].copy());
    }
    return mergedPop;
}

bool NSGA_II::isDiverse() {
    if (doMutation){
        return true;
    } else {
        return GA::isDiverse();
    }
}


GA* NSGA_II::clone() const {
    return new NSGA_II(static_cast<const NSGA_II&>(*this));
}

string NSGA_II::id(){
    return "NSGA-II";
}

void NSGA_II::draw2DVisualization(vector<Individual> &population, int maxX, int maxY){
    vector<Individual*> drawList;
    drawList.reserve(population.size());
    for (int i = 0; i < population.size(); i++){
        drawList.push_back(&population[i]);
    }
    sort(drawList.begin(), drawList.end(), [](const Individual* lhs, const Individual* rhs){
        if (lhs->fitness[1] < rhs->fitness[1]){
            return true;
        } else if (lhs->fitness[1] > rhs->fitness[1]){
            return false;
        } else {
            return lhs->fitness[0] < rhs->fitness[0];
        }
    });
    int i = 0;
    string result = "";
    for (int y = 0; y < maxY; y++){
        for (int x = 0; x < maxX; x++){
            if (drawList[i]->fitness[0] == x && drawList[i]->fitness[1] == y){
//                string reproduce = drawList[i]->canReproduce ? "+" : "";
                string frontNr = drawList[i]->front == -1 ? "?" : to_string(drawList[i]->front);
                result += Utility::padWithSpacesAfter(frontNr, 3);
            } else {
                result += " . ";
            }
            result += " ";
            while (drawList[i]->fitness[0] == x && drawList[i]->fitness[1] == y && i < drawList.size()-1){
                i++;
            }
        }
        result += "\n";
    }
    cout << result << endl;
}
