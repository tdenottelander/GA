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

UniqueSolutions::UniqueSolutions(int alphabetSize) : genotypes(unordered_set<int>()), alphabetSize(alphabetSize) {
}

void UniqueSolutions::put(uvec &genotype){
    genotypes.insert(hash(genotype));
}

bool UniqueSolutions::contains(uvec &genotype){
    int hashresult = hash(genotype);
    return (genotypes.find(hash(genotype)) != genotypes.end());
}

int UniqueSolutions::hash(uvec &genotype){
    int result = 0;
    int n = genotype.size();
    for (int i = 0; i < n; i++){
        result += genotype[i] * pow(alphabetSize, n-i-1);
    }
    return result;
}

//struct uvecHash {
//public:
//    size_t operator()(const arma::uvec & genotype) const {
//        int result = 0;
//        int n = genotype.size();
//        int alphabetSize = 3;
//        for (int i = 0; i < alphabetSize; i++){
//            result += genotype[i] * pow(alphabetSize, n-i-1);
//        }
//        return std::hash<int>()(result);
//    }
//};
//
//struct uvecEqual {
//public:
//    bool operator()(const arma::uvec & g1, const arma::uvec & g2) const {
//        if(g1[0] == g2[0])
//            return true;
//        else
//            return false;
//    }
//};
