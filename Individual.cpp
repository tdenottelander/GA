#include "Individual.h"

using namespace arma;
using namespace std;


/* ------------------------ Individual ------------------------ */

// uvec genotype;
// int fitness = -1;

Individual::Individual(int length){
    genotype = uvec (length);
}

void Individual::initialize(){
    for(int i = 0; i < genotype.size(); i++){
        genotype[i] = ((float)rand() / RAND_MAX) < 0.5 ? 1 : 0;
    }
}

Individual Individual::copy(){
    int l = genotype.size();
    Individual ind(l);
    for(int i = 0; i < l; i++){
        ind.genotype[i] = genotype[i];
    }
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
    result += "]";
    return result;
}
