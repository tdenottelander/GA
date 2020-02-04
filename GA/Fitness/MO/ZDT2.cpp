//
//  ZDT2.cpp
//  GA
//
//  Created by Tom den Ottelander on 04/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ZDT2.hpp"

using namespace std;

ZDT2::ZDT2 () : FitnessFunction(new BinaryProblemType()) {
    setLength(30);
    setNumObjectives(2);
}

vector<float> ZDT2::evaluate(Individual &ind) {
    vector<float> result (numObjectives, 0);
    result[0] = ind.genotype[0];
    
    
    float gSub = 0;
    for(int i = 1; i < ind.genotype.size(); i++){
        gSub += ind.genotype[i];
    }
    float g = 1.0 + 9.0 * gSub / (totalProblemLength - 1);
    float f2 = g * (1 - sqrt(ind.genotype[0] / g));
    result[1] = f2;
    
    ind.fitness = result;
    
    evaluationProcedure(ind);
    
    return result;
}

FitnessFunction* ZDT2::clone() const {
    return new ZDT2(static_cast<const ZDT2&>(*this));
}
