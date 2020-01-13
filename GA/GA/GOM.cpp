//
//  GOM.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "GOM.hpp"

using namespace std;

GOM::GOM(FitnessFunction * fitfunc, FOS *fos, bool forcedImprovement = false) : GA(fitfunc), fos(fos), gom_variation(*new GOM_Variation(forcedImprovement)) {
}

void GOM::initialize() {
    GA::initialize();
    gom_variation.fitfunc = fitFunc_ptr;
    gom_variation.setFOSObject(fos);
}

void GOM::round() {
    //TODO: Make the evaluations stop when the max number of evaluations is reached
    vector<Individual> offspring = gom_variation.variate(population);
    
    population = offspring;
    
    roundsCount++;
}

GA* GOM::clone() const {
    return new GOM(static_cast<const GOM&>(*this));
}

string GOM::id(){
    return "GOM_" + fos->id(); // + "_FI=" + (gom_variation.forcedImprovement ? "t" : "f");
}
