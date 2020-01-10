//
//  LocalSearchBase.cpp
//  GA
//
//  Created by Tom den Ottelander on 07/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "LocalSearchBase.hpp"

using namespace std;

LocalSearchBase::LocalSearchBase(){}

LocalSearchBase::LocalSearchBase (FitnessFunction * fitFunc, Utility::Order localSearchOrder, float stochasticity) : localSearchOrder(localSearchOrder), stochasticity(stochasticity){
    fitFunc_ptr = fitFunc;
}

void LocalSearchBase::round(){
    
    for(Individual ind : population){
        
        doLocalSearch(ind, fitFunc_ptr);
        
        if(fitFunc_ptr->optimumFound)
            break;
    }
    roundsCount++;
}

void LocalSearchBase::doLocalSearch(Individual &ind, FitnessFunction * fitfunc, int maxRandomFlips){
    bool converged = false;
    int randomFlips = 0;
    vector<int> alphabet = fitfunc->problemType->alphabet;
    
    while(!converged){
        converged = true;
        
        int probSize = fitfunc->totalProblemLength;
        vector<int> order = Utility::getOrderedArray(probSize, localSearchOrder);
        
        for(int index : order){
            
            int originalLayer = ind.genotype[index];
            int newLayer = originalLayer;
            
            if((randomFlips < maxRandomFlips || maxRandomFlips == -1) && Utility::getRand() < stochasticity){
                newLayer = Utility::getRand(0, alphabet.size());
                ind.genotype[index] = newLayer;
                fitfunc->evaluate(ind);
                randomFlips += 1;
            } else {
            
                float bestFitness = ind.fitness;
                
                for(int layertype : alphabet){
                    if(layertype != originalLayer) {
                        ind.genotype[index] = layertype;
                        float fitness = fitfunc->evaluate(ind);
                        if(fitness > bestFitness){
                            newLayer = layertype;
                            bestFitness = fitness;
                        }
                    }
                }
                
                ind.genotype[index] = newLayer;
                ind.fitness = bestFitness;
            }
            
            if (newLayer != originalLayer){
                converged = false;
            }
        }
    }
}
