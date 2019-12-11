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

Individual::Individual() : fitness(-1), counterNotChanged(0){}

Individual::Individual(int length) : fitness(-1), counterNotChanged(0){
    genotype = uvec (length);
}

void Individual::initialize(vector<int> alphabet){
    int n = alphabet.size();
    for(unsigned long long i = 0; i < genotype.size(); i++){
        genotype[i] = alphabet[floor(getRand() * n)];
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
    for (unsigned long i = 0; i < genotype.size(); i++){
        result += to_string(genotype[i]);
        if(i != (genotype.size() - 1)){
            result += " ";
        }
    }
    result += "]  F: ";
    result += to_string(fitness);
    return result;
}

bool Individual::equals(const Individual &ind) {
    if(fitness != ind.fitness){
        return false;
    }
    for (unsigned long i = 0; i < genotype.size(); i++){
        if(genotype[i] != ind.genotype[i]){
            return false;
        }
    }
    return true;
}
