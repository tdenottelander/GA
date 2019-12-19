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

// Constructor
UniqueSolutions::UniqueSolutions(int alphabetSize) : genotypes(unordered_set<int>()), alphabetSize(alphabetSize) {
}

// Inserts a hashed genotype into the unorderd set, regardless of already being in it
void UniqueSolutions::put(uvec &genotype){
    genotypes.insert(hash(genotype));
}

// Returns true when the unordered set already contains the hash if this genotype
bool UniqueSolutions::contains(uvec &genotype){
    return (genotypes.find(hash(genotype)) != genotypes.end());
}

// Hashes an individual's genotype by enumeration
int UniqueSolutions::hash(uvec &genotype){
    int result = 0;
    int n = genotype.size();
    for (int i = 0; i < n; i++){
        result += genotype[i] * pow(alphabetSize, n-i-1);
    }
    return result;
}
