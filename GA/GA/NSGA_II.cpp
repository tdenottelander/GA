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

NSGA_II::NSGA_II(FitnessFunction * fitFunc, Variation * var, int tournamentSize, float crossoverProbability, bool mutation, bool visualize) : SimpleGA(fitFunc, var, NULL), tournamentSize(tournamentSize), crossoverProbability(crossoverProbability), mutation(mutation), visualize(visualize) {
}

void NSGA_II::round() {
    if(initialRound){
        clearMOinformation(population);
        sortedPopulation = nonDominatedSorting(population);
        draw2DVisualization(population, fitFunc_ptr->optimum[0]+1, fitFunc_ptr->optimum[1]+1);
        initialRound = false;
    }
    vector<Individual*> Pt = selectPt(sortedPopulation);
    vector<Individual> Qt = createOffspring(Pt);
    
    vector<Individual> newPopulation;
    for (int i = 0; i < Pt.size(); i++){
        newPopulation.push_back(Pt[i]->copy());
    }
    for (int i = 0; i < Qt.size(); i++){
        newPopulation.push_back(Qt[i]);
    }
    population.clear();
    population = newPopulation;
    
    vector<float> averageFitness = getAvgFitness();
//    cout << "Avg Fitness = " << Utility::wrapWithBrackets(Utility::vecOfFloatsToString(averageFitness, ", ")) << endl;
    
    clearMOinformation(population);
    sortedPopulation = nonDominatedSorting(population);
    draw2DVisualization(population, fitFunc_ptr->optimum[0]+1, fitFunc_ptr->optimum[1]+1);
    
    fitFunc_ptr->updateElitistArchive(sortedPopulation[0]);
    
    roundsCount++;
}

void NSGA_II::clearMOinformation(vector<Individual> &population){
    for (Individual ind : population){
        ind.clearMOinformation();
    }
}

// Sorts the individuals into fronts. If n is not -1, then there process of sorting into fronts stops when
// finding all the solutions of a front is done and the total amount of solutions is at least n.
vector<vector<Individual*>> NSGA_II::nonDominatedSorting (vector<Individual> &population, int n){
    
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
        // Sort individuals in this front on fitness for objective [obj].
        sort(front.begin(), front.end(), CandidateObjectiveComparator(obj));
        
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
    }
    
    sort(front.begin(), front.end(), [](const Individual* rhs, const Individual* lhs){
        return lhs->crowdingDistance < rhs->crowdingDistance;
    });
}

// Returns true if can1 is favoured over can2
bool NSGA_II::crowdComparisonOperator(const Individual* can1, const Individual* can2){
    if (can1->front < can2->front){
        return true;
    } else if (can1->front > can2->front){
        return false;
    } else {
        return can1->crowdingDistance > can2->crowdingDistance;
    }
}

vector<Individual*> NSGA_II::selectPt(vector<vector<Individual*>> sortedPopulation){
    vector<Individual*> Pt;
    Pt.reserve(populationSize);
    int individualsAdded = 0;
    
    for (vector<Individual*> &front : sortedPopulation){
        CrowdingDistanceSorting(front);
        for (int i = 0; i < front.size(); i++){
            front[i]->canReproduce = true;
            Pt.push_back(front[i]);
//            cout << individualsAdded << ":  front=" << front[i]->front << "  crDist=" << front[i]->crowdingDistance << endl;
            if (++individualsAdded == populationSize){
                return Pt;
            }
        }
    }
    return Pt;
}

vector<Individual> NSGA_II::createOffspring(vector<Individual*> Pt){
    vector<Individual> Qt;
    Qt.reserve(populationSize);

    if (populationSize % 4 != 0){
        cout << "Consider setting the population size to a multiple of 4.";
    }
    
    for (int i = 0; i < 2; i++){
        vector<int> randIdxArray = Utility::getRandomlyPermutedArrayV2(populationSize);
        
        for (int j = 0; j < populationSize/4; j++){
            Individual* parent1 = Pt[randIdxArray[j * 4 + 0]];
            Individual* parent2 = Pt[randIdxArray[j * 4 + 1]];
            Individual* parent3 = Pt[randIdxArray[j * 4 + 2]];
            Individual* parent4 = Pt[randIdxArray[j * 4 + 3]];
            
            Individual *firstParent;
            Individual *secondParent;
            if (crowdComparisonOperator(parent1, parent2)){
                firstParent = parent1;
            } else {
                firstParent = parent2;
            }
            
            if (crowdComparisonOperator(parent3, parent4)){
                secondParent = parent3;
            } else {
                secondParent = parent4;
            }
            
            Individual newInd1;
            Individual newInd2;
            if (Utility::getRand() < crossoverProbability){
                pair<Individual, Individual> offspring = variation_ptr->crossover(*firstParent, *secondParent);
                newInd1 = offspring.first;
                newInd2 = offspring.second;
            } else {
                newInd1 = firstParent->copy();
                newInd2 = secondParent->copy();
            }
            
            if(mutation){
                float mutationProbability = 1.0 / fitFunc_ptr->totalProblemLength;
                mutate(newInd1, mutationProbability);
                mutate(newInd2, mutationProbability);
            }
            
            fitFunc_ptr->evaluate(newInd1);
            fitFunc_ptr->evaluate(newInd2);
            
            Qt.push_back(newInd1);
            Qt.push_back(newInd2);
        }
    }
    return Qt;
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


GA* NSGA_II::clone() const {
    return new NSGA_II(static_cast<const NSGA_II&>(*this));
}

string NSGA_II::id(){
    return "NSGA-II";
}

void NSGA_II::draw2DVisualization(vector<Individual> &population, int maxX, int maxY){
    if(!visualize){
        return;
    }
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

//string NSGA_II::Candidate::toString(){
//    string result = ind->toString();
//    result += "  Front: ";
//    result += to_string(front);
//    result += "  CrowdingDist: ";
//    result += to_string(crowdingDistance);
//    return result;
//}
//
//void NSGA_II::Candidate::print(){
//    cout << toString() << endl;
//}
