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

LocalSearch::LocalSearch (FitnessFunction * fitFunc, Utility::Order localSearchOrder) : localSearchOrder(localSearchOrder){
    fitFunc_ptr = fitFunc;
}

void LocalSearch::round(){
    
    for(Individual ind : population){
        
        doLocalSearch(ind, fitFunc_ptr);
        
        if(fitFunc_ptr->optimumFound)
            break;
    }
    roundsCount++;
}

void LocalSearch::doLocalSearch(Individual &ind, FitnessFunction * fitfunc){
    bool converged = false;
    
    while(!converged){
        converged = true;
        
        int probSize = fitFunc_ptr->totalProblemLength;
        vector<int> order = Utility::getOrderedArray(probSize, localSearchOrder);
        
        for(int index : order){
            
            int originalLayer = ind.genotype[index];
            int bestLayer = originalLayer;
            float bestFitness = ind.fitness;
            
            for(int layertype : fitfunc->problemType->alphabet){
                if(layertype != originalLayer) {
                    ind.genotype[index] = layertype;
                    float fitness = fitfunc->evaluate(ind);
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
}

GA* LocalSearch::clone() const {
    return new LocalSearch(static_cast<const LocalSearch&>(*this));
}

string LocalSearch::id() {
    return ("LocalSearch-" + Utility::orderToID(localSearchOrder));
}
