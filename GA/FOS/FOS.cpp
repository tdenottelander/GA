//
//  FOS.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "FOS.hpp"
using namespace std;
using namespace arma;


/* ------------------------ Base class FOS ------------------------ */

//vector<uvec> FOS::getFOS (int n){ vector<uvec> x; return x; }
string FOS::id(){ return "base FOS"; }
string FOS::toString() { return "baseFOS"; }


/* ------------------------ Incremental LT FOS ------------------------ */

IncrementalLT_FOS::IncrementalLT_FOS(){
    reinitializeOnNewRound = false;
}

vector<uvec> IncrementalLT_FOS::getFOS (vector<Individual> &population){
    return FOSStructures::getIncrementalLT_FOS(population[0].genotype.size());
}
string IncrementalLT_FOS::id(){ return "incrLT"; }
string IncrementalLT_FOS::toString() { return "Incremental Linkage Tree FOS"; }


/* ------------------------ Univariate FOS ------------------------ */

UnivariateOrdered_FOS::UnivariateOrdered_FOS(){
    reinitializeOnNewRound = false;
}

vector<uvec> UnivariateOrdered_FOS::getFOS (vector<Individual> &population){
    return FOSStructures::getUnivariate_FOS(population[0].genotype.size());
}
string UnivariateOrdered_FOS::id(){ return "UniOrd"; }
string UnivariateOrdered_FOS::toString() { return "Univariate (ordered) FOS"; }


/* ------------------------ Incremental LT + Univariate FOS ------------------------ */

IncrementalLT_Univariate_FOS::IncrementalLT_Univariate_FOS(){
    reinitializeOnNewRound = false;
}

vector<uvec> IncrementalLT_Univariate_FOS::getFOS (vector<Individual> &population){
    int n = population[0].genotype.size();
    vector<uvec> fos;
    fos.reserve(2 * n);
    vector<uvec> fixedLT = FOSStructures::getIncrementalLT_FOS(n);
    vector<uvec> univariate = FOSStructures::getUnivariate_FOS(n);
    fos.insert(fos.begin(), fixedLT.begin(), fixedLT.end());
    fos.insert(fos.end(), univariate.begin(), univariate.end());
    return fos;
}
string IncrementalLT_Univariate_FOS::id() { return "incrLT-UniOrd"; }
string IncrementalLT_Univariate_FOS::toString(){ return "Incremental Linkage Tree, followed by (ordered) Univariate FOS"; }



/* ------------------------ Unordered Univariate FOS ------------------------ */

Univariate_FOS::Univariate_FOS(){
    reinitializeOnNewRound = true;
}

vector<uvec> Univariate_FOS::getFOS (vector<Individual> &population){
    return FOSStructures::getRandomUnivariate_FOS(population[0].genotype.size());
}

string Univariate_FOS::id() { return "UniUnord"; }
string Univariate_FOS::toString(){ return "Unordered Univariate FOS"; }




/* ------------------------ Namespace FOS Structures ------------------------ */

vector<uvec> FOSStructures::getIncrementalLT_FOS(int n){
    vector<uvec> fos;
    fos.reserve(n);
    for(int i = 0; i < n; i++){
        uvec subset (i + 1);
        for(int j = 0; j < (i + 1); j++){
            subset[j] = j;
        }
        fos.push_back(subset);
    }
    return fos;
}

vector<uvec> FOSStructures::getUnivariate_FOS(int n){
    vector<uvec> fos;
    fos.reserve(n);
    for(int i = 0; i < n; i++){
        uvec subset(1);
        subset[0] = i;
        fos.push_back(subset);
    }
    return fos;
}

vector<uvec> FOSStructures::getRandomUnivariate_FOS(int n){
    vector<uvec> fos;
    fos.reserve(n);
    vector<int> randArray = Utility::getRandomlyPermutedArrayV2(n);
    for (int i = 0; i < n; i++){
        uvec subset(1);
        subset[0] = randArray[i];
        fos.push_back(subset);
    }
    return fos;
}

void FOSStructures::printFOS(std::vector<arma::uvec> fos){
    cout << "Learned LT: ";
    for (int i = 0; i < fos.size(); i++){
        cout << "[";
        uvec subset = fos[i];
        for (int j = 0; j < subset.size(); j++){
            cout << subset[j];
            if(j != subset.size() - 1){
                cout << " ";
            }
        }
        cout << "]";
        if (i != fos.size() - 1){
            cout << ",";
        }
    }
    cout << endl;
}
