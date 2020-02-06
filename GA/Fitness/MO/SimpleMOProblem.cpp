//
//  SimpleMOProblem.cpp
//  GA
//
//  Created by Tom den Ottelander on 03/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "SimpleMOProblem.hpp"

using namespace std;

SimpleMOProblem::SimpleMOProblem (int blocksize, int numObjectives) : FitnessFunction(new BinaryProblemType()), blocksize(blocksize) {
    setNumObjectives(numObjectives);
    setLength(blocksize * numObjectives);
    setOptimum(vector<float>(numObjectives, pow(blocksize,2) - 1));
}

vector<float> SimpleMOProblem::evaluate(Individual &ind) {
    vector<float> result (numObjectives, 0);
    for (int i = 0; i < numObjectives; i++){
        for (int j = 0; j < blocksize; j++){
            result[i] += ind.genotype[i * blocksize + j] * pow(2, blocksize-j-1);
            //            result[i] += ind.genotype[i * blocksize + 1] * 1;
        }
    }
    
    ind.fitness = result;
    
    evaluationProcedure(ind);
    
    return result;
}

FitnessFunction* SimpleMOProblem::clone() const {
    return new SimpleMOProblem(static_cast<const SimpleMOProblem&>(*this));
}
