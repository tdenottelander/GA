//
//  NK.cpp
//  GA
//
//  Created by Tom den Ottelander on 14/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "NK.hpp"

using namespace std;
using namespace arma;

NK::NK() : FitnessFunction(-1, -1, getProblemType(3)), blocksize(3), wraparound(false) {
    totalProblemLength = 8;
    createTable();
}

NK::NK(int problemLength, int blocksize, bool wraparound, int alphabetsize, int maxEvaluations) :
    FitnessFunction(-1, maxEvaluations, getProblemType(alphabetsize)),
    blocksize(blocksize),
    wraparound(wraparound)
    {
        totalProblemLength = problemLength;
        createTable();
}

void NK::createTable(){
    cout << "Creating lookup table for NK function. This may take a while.";
    int alphabetSize = problemType->alphabet.size();
    int numSubFunctions = totalProblemLength - blocksize + 1;
    int numPossiblitiesPerSubfunction = pow(alphabetSize, blocksize);
    vector<vector<float>> fitnessLookup;
    fitnessLookup.reserve(numSubFunctions);
    
    for (int i = 0; i < numSubFunctions; i++){
//        cout << "fitnessPerBlock " << i << ":" << endl;
        vector<float> fitnessPerBlock;
        fitnessPerBlock.reserve(numPossiblitiesPerSubfunction);
        for (int hash = 0; hash < numPossiblitiesPerSubfunction; hash++){
            fitnessPerBlock.push_back(Utility::getRand());
            uvec block = HashingFunctions::decode(hash, blocksize, alphabetSize);
//            cout << hash << " -> " << Utility::genotypeToString(block) << ": " << fitnessPerBlock[hash] << endl;
        }
        fitnessLookup.push_back(fitnessPerBlock);
        cout << ".";
    }
    
//    cout << endl;
//    cout << "lookupTable: " << endl;
    float opt = -1;
    int numPossibleGenotypes = pow(alphabetSize, totalProblemLength);
    for (int hash = 0; hash < numPossibleGenotypes; hash++){
        float fitness = 0;
        uvec genotype = HashingFunctions::decode(hash, totalProblemLength, alphabetSize);
//        cout << hash << " -> " << Utility::genotypeToString(genotype) << ": ";
        for (int i = 0; i < numSubFunctions; i++){
            uvec block = genotype.subvec(i, i+2);
            int subhash = HashingFunctions::hash(block, alphabetSize);
            fitness += fitnessLookup[i][subhash];
//            cout << fitnessLookup[i][subhash] << "+";
        }
//        cout << "   = " << fitness << endl;
        lookupTable[Utility::genotypeToString(genotype)] = fitness;
//        cout << "fitness: " << fitness << " > " << "opt: " << opt << " ? " << "result=" << (fitness > opt) << endl;
        if (fitness > opt){
//            cout << "New top fitness: " << fitness << endl;
            opt = fitness;
        }
        if (hash % (hash/10) == 0){
            cout << ".";
        }
    }
    optimum = opt;
    
    cout << "\nOptimum is " << opt << endl;
}

float NK::evaluate(Individual &ind){
    float fitness = lookupTable[Utility::genotypeToString(ind.genotype)];
    ind.fitness = fitness;
    
    evaluationProcedure(ind);
    
    return fitness;
}

ProblemType* NK::getProblemType(int alphabetsize){
    vector<int> alphabet;
    alphabet.reserve(alphabetsize);
    for (int i = 0; i < alphabetsize; i++){
        alphabet.push_back(i);
    }
    return new AlphabetProblemType(alphabet);
}

void NK::display() {
    cout << "NK fitness function with id=[" << id() << "]" << endl;
}

string NK::id() {
    string result = "NK-l=" + to_string(totalProblemLength) + "-bs=" + to_string(blocksize) + "-w=";
    if(wraparound){
        result += "t";
    } else {
        result += "f";
    }
    return result;
}

FitnessFunction* NK::clone() const {
    return new NK(static_cast<const NK&>(*this));
}

void NK::setLength(int length){
    totalProblemLength = length;
}
