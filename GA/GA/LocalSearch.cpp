//
//  LocalSearch.cpp
//  GA
//
//  Created by Tom den Ottelander on 07/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "LocalSearch.hpp"

using namespace std;

LocalSearch::LocalSearch(){}

LocalSearch::LocalSearch (FitnessFunction * fitFunc, LocalSearchType localSearchType) : localSearchType(localSearchType){
    fitFunc_ptr = fitFunc;
}

void LocalSearch::round(){
    
    for(Individual ind : population){
        
        bool converged = false;
        
        while(!converged){
            converged = true;
            
            vector<int> order;
            int probSize = fitFunc_ptr->totalProblemLength;
            if(localSearchType == LocalSearchType::RANDOM){
                order = Utility::getRandomlyPermutedArrayV2(probSize);
            } else if (localSearchType == LocalSearchType::ASCENDING){
                order = Utility::getAscendingArray(probSize);
            } else if (localSearchType == LocalSearchType::DESCENDING){
                order = Utility::getDescendingArray(probSize);
            }
            
            for(int index : order){

                int originalLayer = ind.genotype[index];
                int bestLayer = originalLayer;
                float bestFitness = ind.fitness;
                
                for(int layertype : fitFunc_ptr->problemType->alphabet){
                    if(layertype != originalLayer) {
                        ind.genotype[index] = layertype;
                        float fitness = fitFunc_ptr->evaluate(ind);
                        if(fitness > bestFitness){
                            bestLayer = layertype;
                            bestFitness = fitness;
                        }
                    }
                }
                
                ind.genotype[index] = bestLayer;
                ind.fitness = bestFitness;
                                
                if (bestLayer != originalLayer){
                    converged = false;
                }
            }
        }
        
        if(fitFunc_ptr->optimumFound)
            break;
    }
    roundsCount++;
}

GA* LocalSearch::clone() const {
    return new LocalSearch(static_cast<const LocalSearch&>(*this));
}

string LocalSearch::id() {
    string main = "LocalSearch-";
    switch(localSearchType){
        case LocalSearchType::RANDOM:
            return main + "rand";
        case LocalSearchType::ASCENDING:
            return main + "asc";
        case LocalSearchType::DESCENDING:
            return main + "desc";
    }
}
