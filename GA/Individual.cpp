//
//  Individual.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "Individual.hpp"

using namespace arma;
using namespace std;
using namespace Utility;


/* ------------------------ Individual ------------------------ */

// uvec genotype;
// int fitness = -1;

Individual::Individual(int length) : fitness(-1){
    genotype = uvec (length);
}

void Individual::initialize(){
    for(int i = 0; i < genotype.size(); i++){
        genotype[i] = getRand() < 0.5 ? 1 : 0;
    }
}

Individual Individual::copy(){
    int l = genotype.size();
    Individual ind(l);
    for(int i = 0; i < l; i++){
        ind.genotype[i] = genotype[i];
    }
    ind.fitness = fitness;
    return ind;
}

string Individual::toString(){
    string result = "[";
    for (int i = 0; i < genotype.size(); i++){
        result += to_string(genotype[i]);
        if(i != (genotype.size() - 1)){
            result += " ";
        }
    }
    result += "]  F: ";
    result += to_string(fitness);
    return result;
}

bool Individual::operator==(const Individual &ind) {
    for (int i = 0; i < genotype.size(); i++){
        if(genotype[i] != ind.genotype[i]){
            return false;
        }
    }
    return true;
}

bool Individual::operator!=(const Individual &ind) {
    return !(*this == ind);
}
