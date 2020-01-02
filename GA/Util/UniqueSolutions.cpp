//
//  UniqueSolutions.cpp
//  GA
//
//  Created by Tom den Ottelander on 18/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "UniqueSolutions.hpp"

using namespace arma;
using namespace std;
using namespace nlohmann;

// Constructor
UniqueSolutions::UniqueSolutions(int alphabetSize) : genotypes(unordered_set<int>()), alphabetSize(alphabetSize) {
}

// Inserts a hashed genotype into the unorderd set, regardless of already being in it
void UniqueSolutions::put(uvec &genotype){
    genotypes.insert(HashingFunctions::hash(genotype, alphabetSize));
}

// Returns true when the unordered set already contains the hash if this genotype
bool UniqueSolutions::contains(uvec &genotype){
    return (genotypes.find(HashingFunctions::hash(genotype, alphabetSize)) != genotypes.end());
}


SolutionCounter::SolutionCounter (int alphabetSize) : alphabetSize(alphabetSize) {}

void SolutionCounter::put(arma::uvec &genotype){
    int hash = HashingFunctions::hash(genotype, alphabetSize);
    int count = 1;
    if(contains(genotype)){
        count += counterMap.at(hash);
        counterMap.erase(hash);
    }
    pair<int, int> insertion (hash, count);
    counterMap.insert(insertion);
}

bool SolutionCounter::contains(arma::uvec &genotype){
    return (counterMap.find(HashingFunctions::hash(genotype, alphabetSize)) != counterMap.end());
}

int SolutionCounter::get(arma::uvec &genotype){
    if(contains(genotype)){
        return counterMap.at(HashingFunctions::hash(genotype, alphabetSize));
    } else {
        return 0;
    }
}

json SolutionCounter::toJson (bool asHash){
    json result;
    for (auto it = counterMap.begin(); it != counterMap.end(); ++it ){
        if(asHash)
            result[to_string(it->first)] = it->second;
        else {
            uvec genotype = HashingFunctions::decode(it->first, 7, alphabetSize);
            result[Utility::genotypeToString(genotype)] = it->second;
        }
    }
    return result;
}


// Hashes an individual's genotype by enumeration like so:
// 0000000 - 0
// 0000001 - 1
// 0000002 - 2
// 0000010 - 3
// 0000011 - 4
// ...etc
int HashingFunctions::hash(uvec &genotype, int alphabetSize){
    int result = 0;
    int n = genotype.size();
    //    for (int i = 0; i < n; i++){
    //        result += genotype[i] * pow(alphabetSize, n-i-1);
    //    }
    
    for (int i = 0; i < n; i++){
        result += genotype[n-i-1] * pow(alphabetSize, i);
    }
    return result;
}

uvec HashingFunctions::decode(int hash, int problemSize, int alphabetSize){
    uvec genotype(problemSize);
    for(int i = 0; i < problemSize; i++){
        int sub = pow(alphabetSize, problemSize - i - 1);
        int layer = floor(hash / sub);
        genotype[i] = layer;
        hash = hash - layer * sub;
    }
    
    return genotype;
}
