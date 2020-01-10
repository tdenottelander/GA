//
//  GOM_LS.cpp
//  GA
//
//  Created by Tom den Ottelander on 09/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "GOM_LS.hpp"

using namespace std;

GOM_LS::GOM_LS(FitnessFunction * fitfunc, FOS *fos, LocalSearchBase * ls, bool forcedImprovement = false) : GOM(fitfunc, fos, forcedImprovement), ls(ls) {
    fitFunc_ptr = fitfunc;
}

void GOM_LS::round() {
    vector<Individual> offspring = gom_variation.variate(population);
    
    if(fitFunc_ptr->optimumFound)
        return;
    
    // Do local search here
    for(Individual ind : offspring){
        
        ls->doLocalSearch(ind, fitFunc_ptr);
        
        if(fitFunc_ptr->optimumFound)
            return;
    }
    
    population = offspring;
    
    roundsCount++;
}

GA* GOM_LS::clone() const {
    return new GOM_LS(static_cast<const GOM_LS&>(*this));
}

string GOM_LS::id(){
    return ("GOM-LS_" + fos->id() + "_ls-" + Utility::orderToID(ls->localSearchOrder));
}
