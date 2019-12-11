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

Trap::Trap(int blocksize, int blocks) : FitnessFunction(blocks * blocksize), blocks(blocks), blocksize(blocksize) { setProblemType(); }
Trap::Trap(int blocksize) : FitnessFunction(), blocksize(blocksize) { setProblemType(); }

float Trap::evaluate(Individual &ind) {
    float result = 0;
    for (int i = 0; i < blocks; i++) {
        result += subfunc(ind, i, i + blocksize);
    }
    
    ind.fitness = result;
    checkIfBestFound(ind);
    
    evaluations++;
    return result;
}

float Trap::subfunc(Individual &ind, int startIdx, int endIdx) {
    float result = 0;
    for(int i = startIdx; i < endIdx; i++){
        result += ind.genotype[i];
    }
    
    if (result == blocksize){
        return result;
    } else {
        return ((blocksize - 1 - result) / blocksize);
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

void Trap::setProblemType(){
    FitnessFunction::setProblemType(new BinaryProblemType());
}

void Trap::setLength (int length) {
    totalProblemLength = blocksize * length;
    blocks = length;
    optimum = blocksize * length;
}
