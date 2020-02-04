//
//  NSGA_II.cpp
//  GA
//
//  Created by Tom den Ottelander on 03/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "NSGA_II.hpp"

using namespace std;

NSGA_II::NSGA_II(FitnessFunction * fitFunc, Variation * var, Selection * sel) : SimpleGA(fitFunc, var, sel) {
}

void NSGA_II::round() {
    createCandidateStructure();
    nonDominatedSorting();
    
    vector<Individual> newPopulation;
    newPopulation.reserve(populationSize);
    int reproducingPopulationSize = populationSize/2;
    int individualsAdded = 0;
    for (vector<Candidate*> &front : sortedCandidates){
        CrowdingDistanceSorting(front);
        for (int i = 0; i < front.size(); i++){
            front[i]->canReproduce = true;
            newPopulation.push_back(front[i]->ind->copy());
//            cout << individualsAdded << ":  front=" << front[i]->front << "  crDist=" << front[i]->crowdingDistance << endl;
            if (++individualsAdded == reproducingPopulationSize){
                break;
            }
        }
        
        if (individualsAdded == reproducingPopulationSize){
            break;
        }
    }
    
//    draw2DVisualization(16, 16);
    
    if (populationSize % 4 != 0){
        cout << "Consider setting the population size to a multiple of 4.";
    }
        
    for (int i = 0; i < 2; i++){
        vector<int> randIdxArray = Utility::getRandomlyPermutedArrayV2(reproducingPopulationSize);
        for (int j = 0; j < reproducingPopulationSize/4; j++){
            // The selection criteria is first based on rank/front, then on crowding distance.
            // Since all individuals are already sorted on these before, we can just select the individual with the lowest index.
            Individual* firstParent = &newPopulation[min(randIdxArray[j * 2 + 0], randIdxArray[j * 2 + 1])];
            Individual* secondParent = &newPopulation[min(randIdxArray[j * 2 + 2], randIdxArray[j * 2 + 3])];
            
            pair<Individual, Individual> offspring = variation_ptr->crossover(*firstParent, *secondParent);
            
            newPopulation.push_back(offspring.first);
            newPopulation.push_back(offspring.second);
            
            fitFunc_ptr->evaluate(offspring.first);
            fitFunc_ptr->evaluate(offspring.second);
//            cout << offspring.first.toString() << endl;
//            cout << offspring.second.toString() << endl;
        }
    }
    population.clear();
    population = newPopulation;
    vector<float> averageFitness = getAvgFitness();
    cout << "Avg Fitness = " << Utility::wrapWithBrackets(Utility::vecOfFloatsToString(averageFitness, ", ")) << endl;
}

void NSGA_II::createCandidateStructure() {
    candidates.clear();
    candidates.reserve(populationSize);
    // Create candidates for each individual
    for (int i = 0; i < populationSize; i++){
        Candidate can;
        can.ind = &population[i];
        can.dominationCount = 0;
        can.front = -1;
        can.crowdingDistance = 0.0f;
        can.canReproduce = false;
        candidates.push_back(can);
    }
}

void NSGA_II::nonDominatedSorting (){
    
    for (vector<Candidate>::iterator it1 = candidates.begin(); it1 != candidates.end(); it1++){
        for (vector<Candidate>::iterator it2 = (it1 + 1); it2 != candidates.end(); it2++){
            if (it1 == it2){
                continue;
            }
            int domination = dominates(it1->ind, it2->ind);
//            cout << "Domination: " << it1->ind->toString() << ", " << it2->ind->toString() << "    result = " << domination << endl;
            if (domination == 1){
                it2->dominationCount += 1;
                it1->dominationList.push_back(&(*it2));
            } else if (domination == -1){
                it1->dominationCount += 1;
                it2->dominationList.push_back(&(*it1));
            }
        }
    }
    
    list<Candidate*> pool;
    for (int i = 0; i < candidates.size(); i++){
        pool.push_back(&candidates[i]);
    }
    
    sortedCandidates.clear();
    int frontIdx = 0;
    while (!pool.empty()) {
        vector<Candidate*> front;
        for (list<Candidate*>::iterator it = pool.begin(); it != pool.end(); it++){
            if ((*it)->dominationCount == 0){
                (*it)->front = frontIdx;
                front.push_back(*it);
                pool.erase(it);
            }
        }
        
        for (Candidate* can : front){
            for (Candidate *dominatedCandidate : can->dominationList){
                dominatedCandidate->dominationCount -= 1;
            }
        }
        
        sortedCandidates.push_back(front);
        frontIdx++;
    }
}

// Returns 1 if can1 dominates can2
// Returns 0 if neither candidates dominate each other
// Returns -1 if can2 dominates can1
int NSGA_II::dominates(Individual *ind1, Individual *ind2){
    int result = 0;
    for (int i = 0; i < fitFunc_ptr->numObjectives; i++){
        if (ind1->fitness[i] > ind2->fitness[i]){
            if (result == -1){
                return 0;
            }
            result = 1;
        } else if (ind2->fitness[i] > ind1->fitness[i]){
            if (result == 1){
                return 0;
            }
            result = -1;
        }
    }
    return result;
}


void NSGA_II::CrowdingDistanceSorting (vector<Candidate*> &front){
    for (int obj = 0; obj < fitFunc_ptr->numObjectives; obj++){
        sort(front.begin(), front.end(), CrowdingComparator(obj));
        
        front.front()->crowdingDistance = INFINITY;
        front.back()->crowdingDistance = INFINITY;
        
        if (candidates.size() > 2){
            float maxObjFitness = front.back()->ind->fitness[obj];
            float minObjFitness = front.front()->ind->fitness[obj];
            float normalizationValue = maxObjFitness - minObjFitness;
            
            for (int i = 1; i < front.size() - 1; i++){
//                cout << "i = " << i << "  frontNr = " << front[i]->front << endl;
//                cout << "Current ind: " << front[i]->ind->toString() << endl;
                if(front[i]->crowdingDistance != INFINITY){
                    float prevObjFitness = front[i-1]->ind->fitness[obj];
                    float nextObjFitness = front[i+1]->ind->fitness[obj];
//                    cout << "Prev ind: " << front[i-1]->ind->toString() << endl;
//                    cout << "Next ind: " << front[i+1]->ind->toString() << endl;
                    float newCrowdingDistance = -1.0f;
                    if (normalizationValue == 0){
                        newCrowdingDistance = INFINITY;
                    } else {
                        newCrowdingDistance = front[i]->crowdingDistance + ((nextObjFitness - prevObjFitness) / normalizationValue);
                    }
//                    cout << "new Crowding Distance: " << newCrowdingDistance << endl;
                    front[i]->crowdingDistance = newCrowdingDistance;
//                    cout << endl;
                }
            }
        }
    }
    
    sort(front.begin(), front.end(), [](const Candidate* rhs, const Candidate* lhs){
        return lhs->crowdingDistance < rhs->crowdingDistance;
    });
}

GA* NSGA_II::clone() const {
    return new NSGA_II(static_cast<const NSGA_II&>(*this));
}

string NSGA_II::id(){
    return "NSGA-II";
}

void NSGA_II::draw2DVisualization(int maxX, int maxY){
    vector<Candidate*> drawList;
    drawList.reserve(candidates.size());
    for (vector<Candidate*> &front : sortedCandidates){
        for (Candidate* can : front){
            drawList.push_back(can);
        }
    }
    sort(drawList.begin(), drawList.end(), [](const Candidate* lhs, const Candidate* rhs){
        if (lhs->ind->fitness[1] != rhs->ind->fitness[1]){
            return lhs->ind->fitness[1] < rhs->ind->fitness[1];
        } else {
            return lhs->ind->fitness[0] < rhs->ind->fitness[0];
        }
    });
    vector<Candidate*>::iterator it = drawList.begin();
    Candidate* upNext = *it;
    string result = "";
    for (int y = 0; y < maxY; y++){
        for (int x = 0; x < maxX; x++){
            if (upNext->ind->fitness[0] == x && upNext->ind->fitness[1] == y){
                string reproduce = upNext->canReproduce ? "+" : "";
                result += Utility::padWithSpacesAfter(to_string(upNext->front) + reproduce, 3);
            } else {
                result += " . ";
            }
            result += " ";
            while (upNext->ind->fitness[0] == x && upNext->ind->fitness[1] == y && it != drawList.end()){
                upNext = *(it++);
            }
        }
        result += "\n";
    }
    cout << result << endl;
}
