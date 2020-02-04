//
//  CountingOnesMO.cpp
//  GA
//
//  Created by Tom den Ottelander on 04/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "CountingOnesMO.hpp"

using namespace std;

CountingOnesMO::CountingOnesMO (int blocksize, int numObjectives) : FitnessFunction(new BinaryProblemType()), blocksize(blocksize) {
    setLength(blocksize * numObjectives);
    setNumObjectives(numObjectives);
}

vector<float> CountingOnesMO::evaluate(Individual &ind) {
    vector<float> result (numObjectives, 0);
    for (int i = 0; i < numObjectives; i++){
        for (int j = 0; j < blocksize; j++){
            result[i] += ind.genotype[i * blocksize + j];
        }
    }
    
    ind.fitness = result;
    
    return result;
}

FitnessFunction* CountingOnesMO::clone() const {
    return new CountingOnesMO(static_cast<const CountingOnesMO&>(*this));
}

