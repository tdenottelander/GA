//
//  GOM.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "GOM.hpp"

using namespace std;

GOM::GOM(FitnessFunction * fitfunc, FOS *fos, bool forcedImprovement = false) : GA(), fos(fos), gom_variation(*new GOM_Variation(forcedImprovement)) {
    fitFunc_ptr = fitfunc;
}

void GOM::initialize() {
    GA::initialize();
    gom_variation.fitfunc = fitFunc_ptr;
}

void GOM::round() {
    vector<Individual> offspring = gom_variation.variate(population);
    
    population = offspring;
    
    roundsCount++;
}

GA* GOM::clone() const {
    return new GOM(static_cast<const GOM&>(*this));
}

void GOM::setProblemLength (int length) {
    GA::setProblemLength(length);
    gom_variation.setFOSObject(fos);
}

string GOM::id(){
    return "GOM_fos=" + fos->id() + "_FI=" + (gom_variation.forcedImprovement ? "t" : "f");
}
