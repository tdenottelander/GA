//
//  TrapInverseTrap.cpp
//  GA
//
//  Created by Tom den Ottelander on 06/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "TrapInverseTrap.hpp"

using namespace std;

TrapInverseTrap::TrapInverseTrap(int blocksize, int blocks, int numObjectives) : FitnessFunction(vector<float>(numObjectives, blocks * blocksize), new BinaryProblemType()), blocks(blocks), blocksize(blocksize) {
    setLength(blocks);
    setNumObjectives(numObjectives);
}

// Returns the fitness of an individual
vector<float> TrapInverseTrap::evaluate(Individual &ind) {
    vector<float> result (numObjectives, 0.0);
    for (int obj = 0; obj < numObjectives; obj++){
        for (int i = 0; i < blocks; i++) {
            result[obj] += subfunc(ind, i * blocksize, i * blocksize + blocksize, obj);
        }
    }

    ind.fitness = result;
    
    evaluationProcedure(ind);
    
    return result;
}

// Returns the fitness of a block of genes.
float TrapInverseTrap::subfunc(Individual &ind, int startIdx, int endIdx, int objective) {
    int u = 0;
    for(int i = startIdx; i < endIdx; i++){
        if (ind.genotype[i] == objective){
            u += 1;
        }
    }

    if (u == blocksize){
        return blocksize;
    } else {
        return (blocksize - 1.0) - u;
    }
}

void TrapInverseTrap::display() {
    cout << "Trap(" << blocksize << ") fitness function" << endl;
}

string TrapInverseTrap::id() {
    return ("T" + to_string(blocksize));
}


FitnessFunction* TrapInverseTrap::clone() const {
    return new TrapInverseTrap(static_cast<const TrapInverseTrap&>(*this));
}

void TrapInverseTrap::setLength (int length) {
    totalProblemLength = blocksize * length;
    blocks = length;
    setOptimum(vector<float>(numObjectives, totalProblemLength));
}

void TrapInverseTrap::setOptimum (vector<float> opt) {
    optimum = opt;
    optimalParetoFrontSize = blocks + 1;
}
