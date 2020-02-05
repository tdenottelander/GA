//
//  Trap.cpp
//  GA
//
//  Created by Tom den Ottelander on 11/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "Trap.hpp"

using namespace std;

/* ------------------------ Trap Five Fitness Function ------------------------ */

Trap::Trap(int blocksize, int blocks) : FitnessFunction(vector<float>(1, blocks * blocksize), getProblemType()), blocks(blocks), blocksize(blocksize) {}
Trap::Trap(int blocksize) : FitnessFunction(getProblemType()), blocksize(blocksize) {}

// Returns the fitness of an individual
vector<float> Trap::evaluate(Individual &ind) {
    vector<float> result (1, 0.0);
    for (int i = 0; i < blocks; i++) {
        result[0] += subfunc(ind, i * blocksize, i * blocksize + blocksize);
    }
    
    ind.fitness[0] = result[0];
    evaluationProcedure(ind);
    return result;
}

// Returns the fitness of a block of genes.
float Trap::subfunc(Individual &ind, int startIdx, int endIdx) {
    int ones = 0;
    for(int i = startIdx; i < endIdx; i++){
        ones += ind.genotype[i];
    }
    
    if (ones == blocksize){
        return 1.0;
    } else {
        return ((blocksize - 1.0 - ones) / (1.0 * blocksize));
    }
}

FitnessFunction* Trap::clone() const {
    return new Trap(static_cast<const Trap&>(*this));
}

void Trap::display() {
    cout << "Trap(" << blocksize << ") fitness function" << endl;
}

string Trap::id() {
    return ("T" + to_string(blocksize));
}

ProblemType* Trap::getProblemType(){
    return new BinaryProblemType();
}

void Trap::setLength (int length) {
    totalProblemLength = blocksize * length;
    blocks = length;
    setOptimum(length);
}
