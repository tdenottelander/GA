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

IncrementalLT_FOS::IncrementalLT_FOS(){}

vector<uvec> IncrementalLT_FOS::getFOS (int n){
    return FOSStructures::getIncrementalLT_FOS(n);
}
string IncrementalLT_FOS::id(){ return "incrLT"; }
string IncrementalLT_FOS::toString() { return "Incremental Linkage Tree FOS"; }


/* ------------------------ Univariate FOS ------------------------ */

Univariate_FOS::Univariate_FOS(){}

vector<uvec> Univariate_FOS::getFOS (int n){
    return FOSStructures::getUnivariate_FOS(n);
}
string Univariate_FOS::id(){ return "UniOrd"; }
string Univariate_FOS::toString() { return "Univariate (ordered) FOS"; }


/* ------------------------ Incremental LT + Univariate FOS ------------------------ */

IncrementalLT_Univariate_FOS::IncrementalLT_Univariate_FOS(){}

vector<uvec> IncrementalLT_Univariate_FOS::getFOS (int n){
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
