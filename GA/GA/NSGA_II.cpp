//
//  NSGA_II.cpp
//  GA
//
//  Created by Tom den Ottelander on 03/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "NSGA_II.hpp"

using namespace std;

NSGA_II::NSGA_II(FitnessFunction * fitFunc, Variation * var, Selection * sel, bool visualize) : SimpleGA(fitFunc, var, sel), visualize(visualize) {
}

void NSGA_II::round() {
    vector<Candidate> candidates = createCandidateStructure(population);
    vector<vector<Candidate*>> sortedCandidates = nonDominatedSorting(candidates, populationSize);
    vector<Candidate*> Pt = selectPt(sortedCandidates);
    vector<Individual> Qt = createOffspring(Pt);

    if(visualize){
        draw2DVisualization(candidates, fitFunc_ptr->optimum[0]+1, fitFunc_ptr->optimum[1]+1);
    }
    
    vector<Individual> newPopulation;
    for (int i = 0; i < Pt.size(); i++){
        newPopulation.push_back(Pt[i]->ind->copy());
    }
    for (int i = 0; i < Qt.size(); i++){
        newPopulation.push_back(Qt[i]);
    }
    population.clear();
    population = newPopulation;
    
    vector<float> averageFitness = getAvgFitness();
//    cout << "Avg Fitness = " << Utility::wrapWithBrackets(Utility::vecOfFloatsToString(averageFitness, ", ")) << endl;
}

// Creates a candidate struct for every individual, which stores the dominationcount, frontrank, crowdingdistance and whether it can reproduce.
vector<NSGA_II::Candidate> NSGA_II::createCandidateStructure(vector<Individual> &population) {
    vector<Candidate> candidates;
    candidates.reserve(population.size());
    for (int i = 0; i < population.size(); i++){
        Candidate can;
        can.ind = &population[i];
        can.dominationCount = 0;
        can.front = -1;
        can.crowdingDistance = 0.0f;
        can.canReproduce = false;
        candidates.push_back(can);
    }
    return candidates;
}

// Sorts the candidates into fronts. If n is not -1, then there process of sorting into fronts stops when
// finding all the solutions of a front is done and the total amount of solutions is at least n.
vector<vector<NSGA_II::Candidate*>> NSGA_II::nonDominatedSorting (vector<Candidate> &candidates, int n){
    
    // Loop over every candidate combination to see which candidates dominate which other candidates.
    for (int i = 0; i < candidates.size(); i++){
        for (int j = 0; j < candidates.size(); j++){
            if (dominates(candidates[i].ind, candidates[j].ind)){
                // Individual i dominates individual j
                candidates[j].dominationCount += 1;
                candidates[i].dominationList.push_back(&candidates[j]);
            }
        }
    }
    
    // Put a pointer to every candidate in a pool that we can iterate over and modify.
    list<Candidate*> pool;
    for (int i = 0; i < candidates.size(); i++){
        pool.push_back(&candidates[i]);
    }
    
    vector<vector<Candidate*>> sortedCandidates;
    int frontIdx = 0;
    int candidateCount = 0;
    // See which candidates have domination count 0. Put these in the same front.
    while (!pool.empty()) {
        vector<Candidate*> front;
        for (list<Candidate*>::iterator it = pool.begin(); it != pool.end(); it++){
            if ((*it)->dominationCount == 0){
                (*it)->front = frontIdx;
                front.push_back(*it);
                pool.erase(it);
                candidateCount++;
            }
        }
        
        // Modify domination count of other candidates by subtracting the dominates of dominated solutions by solutions from this front.
        for (Candidate* can : front){
            for (Candidate *dominatedCandidate : can->dominationList){
                dominatedCandidate->dominationCount -= 1;
            }
        }
        
        sortedCandidates.push_back(front);
        frontIdx++;
        // If we only need [n] candidates, we can stop assigning fronts.
        if(n != -1 && candidateCount >= n){
            break;
        }
    }
    return sortedCandidates;
}

// Returns true if [ind1] dominates [ind2]
// Notion of domination used: When all objectives are equal or better and at least one objective is strictly better.
bool NSGA_II::dominates(Individual *ind1, Individual *ind2){
    bool strictlyBetter = false;
    for (int obj = 0; obj < fitFunc_ptr->numObjectives; obj++){
        if (ind1->fitness[obj] > ind2->fitness[obj]){
            strictlyBetter = true;
        } else if (ind1->fitness[obj] < ind2->fitness[obj]){
            return false;
        }
    }
    return strictlyBetter;
}

// Sorts the candidates in a front based on the crowding distance.
void NSGA_II::CrowdingDistanceSorting (vector<Candidate*> &front){
    for (int obj = 0; obj < fitFunc_ptr->numObjectives; obj++){
        // Sort individuals in this front on fitness for objective [obj].
        sort(front.begin(), front.end(), CandidateObjectiveComparator(obj));
        
        front.front()->crowdingDistance = INFINITY;
        front.back()->crowdingDistance = INFINITY;
        
        if (front.size() > 2){
            float maxObjFitness = front.back()->ind->fitness[obj];
            float minObjFitness = front.front()->ind->fitness[obj];
            float normalizationValue = maxObjFitness - minObjFitness;
            
            for (int i = 1; i < front.size() - 1; i++){
                if(front[i]->crowdingDistance != INFINITY){
                    float prevObjFitness = front[i-1]->ind->fitness[obj];
                    float nextObjFitness = front[i+1]->ind->fitness[obj];
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
    
    sort(front.begin(), front.end(), [](const Candidate* rhs, const Candidate* lhs){
        return lhs->crowdingDistance < rhs->crowdingDistance;
    });
}

// Returns true if can1 is favoured over can2
bool NSGA_II::crowdComparisonOperator(const Candidate* can1, const Candidate* can2){
    if (can1->front < can2->front){
        return true;
    } else if (can1-> front > can2->front){
        return false;
    } else {
        return can1->crowdingDistance > can2->crowdingDistance;
    }
}

vector<NSGA_II::Candidate*> NSGA_II::selectPt(vector<vector<Candidate*>> sortedCandidates){
    vector<Candidate*> Pt;
    Pt.reserve(populationSize);
    int individualsAdded = 0;
    
    for (vector<Candidate*> &front : sortedCandidates){
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

vector<Individual> NSGA_II::createOffspring(vector<Candidate*> Pt){
    vector<Individual> Qt;
    Qt.reserve(populationSize);

    if (populationSize % 4 != 0){
        cout << "Consider setting the population size to a multiple of 4.";
    }
    
    for (int i = 0; i < 2; i++){
        vector<int> randIdxArray = Utility::getRandomlyPermutedArrayV2(populationSize);
        
        for (int j = 0; j < populationSize/4; j++){
            Candidate* parent1 = Pt[randIdxArray[j * 4 + 0]];
            Candidate* parent2 = Pt[randIdxArray[j * 4 + 1]];
            Candidate* parent3 = Pt[randIdxArray[j * 4 + 2]];
            Candidate* parent4 = Pt[randIdxArray[j * 4 + 3]];
            
            Candidate *firstParent;
            Candidate *secondParent;
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
            
            pair<Individual, Individual> offspring = variation_ptr->crossover(*firstParent->ind, *secondParent->ind);
            
            fitFunc_ptr->evaluate(offspring.first);
            fitFunc_ptr->evaluate(offspring.second);
            
            Qt.push_back(offspring.first);
            Qt.push_back(offspring.second);
        }
    }
    return Qt;
}


GA* NSGA_II::clone() const {
    return new NSGA_II(static_cast<const NSGA_II&>(*this));
}

string NSGA_II::id(){
    return "NSGA-II";
}

void NSGA_II::draw2DVisualization(vector<Candidate> &candidates, int maxX, int maxY){
    vector<Candidate*> drawList;
    drawList.reserve(candidates.size());
    for (int i = 0; i < candidates.size(); i++){
        drawList.push_back(&candidates[i]);
    }
    sort(drawList.begin(), drawList.end(), [](const Candidate* lhs, const Candidate* rhs){
        if (lhs->ind->fitness[1] < rhs->ind->fitness[1]){
            return true;
        } else if (lhs->ind->fitness[1] > rhs->ind->fitness[1]){
            return false;
        } else {
            return lhs->ind->fitness[0] < rhs->ind->fitness[0];
        }
    });
    int i = 0;
    string result = "";
    for (int y = 0; y < maxY; y++){
        for (int x = 0; x < maxX; x++){
            if (drawList[i]->ind->fitness[0] == x && drawList[i]->ind->fitness[1] == y){
//                string reproduce = drawList[i]->canReproduce ? "+" : "";
                string frontNr = drawList[i]->front == -1 ? "?" : to_string(drawList[i]->front);
                result += Utility::padWithSpacesAfter(frontNr, 3);
            } else {
                result += " . ";
            }
            result += " ";
            while (drawList[i]->ind->fitness[0] == x && drawList[i]->ind->fitness[1] == y && i < drawList.size()-1){
                i++;
            }
        }
        result += "\n";
    }
    cout << result << endl;
}

string NSGA_II::Candidate::toString(){
    string result = ind->toString();
    result += "  Front: ";
    result += to_string(front);
    result += "  CrowdingDist: ";
    result += to_string(crowdingDistance);
    return result;
}

void NSGA_II::Candidate::print(){
    cout << toString() << endl;
}
