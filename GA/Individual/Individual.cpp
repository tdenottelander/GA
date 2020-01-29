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
    string result = toString(genotype);
    result += "  F: ";
    result += to_string(fitness);
    return result;
}

string Individual::toStringBlocks(int blocksize){
    string result = "[";
    for (unsigned long i = 0; i < genotype.size(); i++){
        result += to_string(genotype[i]);
        if(i != (genotype.size() - 1)){
            result += " ";
            if((i+1) % blocksize == 0){
                result += "| ";
            }
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

bool Individual::genotypeEquals(uvec &g){
    return genotypeEquals(g, genotype);
}

string Individual::toString(vector<int> &genotype){
    string result = "[";
    for (unsigned long i = 0; i < genotype.size(); i++){
        result += to_string(genotype[i]);
        if(i != (genotype.size() - 1)){
            result += " ";
        }
    }
    result += "]";
    return result;
}

string Individual::toString(uvec &genotype){
    vector<int> gen = Utility::uvecToVector(genotype);
    return toString(gen);
}

int Individual::hammingDistance(uvec &g1, uvec &g2){
    if (g1.size() != g2.size()){
        cout << "Error: Cannot compare hamming distance between two genotypes of different length" << endl;
        return -1;
    }
    int distance = 0;
    for (int i = 0; i < g1.size(); i++){
        if (g1[i] != g2[i])
            distance++;
    }
    return distance;
}

int Individual::editDistance(uvec &g1, uvec &g2){
    return editDistance(g1, g2, g1.size(), g2.size());
}

int Individual::editDistance(uvec &g1, uvec &g2, int i, int j){
    if(i == 0 || j == 0){
        return max(i, j);
    } else {
        int minCase1 = editDistance(g1, g2, i-1, j) + 1;
        int minCase2 = editDistance(g1, g2, i, j-1) + 1;
        int indicator = 1;
        if(g1[i-1] == g2[j-1])
            indicator = 0;
        int minCase3 = editDistance(g1, g2, i-1, j-1) + indicator;
        return min(minCase1, min(minCase2, minCase3));
    }
}

uvec Individual::removeIdentities(uvec &genotype, int identityLayerIndex){
    uvec newGenotype (genotype.size());
    int j = 0;
    for (int i = 0; i < genotype.size(); i++){
        if (genotype[i] != identityLayerIndex){
            newGenotype[j] = genotype[i];
            j++;
        }
    }
    return newGenotype.head(j);
}

bool Individual::genotypeEquals(uvec &g1, uvec &g2){
    if(g1.size() != g2.size()){
        return false;
    }
    for (int i = 0; i < g1.size(); i++){
        if (g1[i] != g2[i]){
            return false;
        }
    }
    return true;
}
