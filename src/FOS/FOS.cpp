//
//  FOS.cpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "FOS.hpp"
using namespace std;

/* ------------------------ Base class FOS ------------------------ */

vector<vector<int>> FOS::getFOS (vector<Individual> &population){
    return getFOS(population[0].genotype.size());
}
string FOS::id(){ return "base FOS"; }
string FOS::toString() { return "baseFOS"; }


/* ------------------------ Incremental LT FOS ------------------------ */

IncrementalLT_FOS::IncrementalLT_FOS(){
    reinitializeOnNewRound = false;
}

vector<vector<int>> IncrementalLT_FOS::getFOS (int genotypeLength){
    return FOSStructures::getIncrementalLT_FOS(genotypeLength);
}
string IncrementalLT_FOS::id(){ return "incrLT-asc"; }
string IncrementalLT_FOS::toString() { return "Ascending Incremental Linkage Tree FOS"; }


/* ------------------------ Incremental LT Reversed FOS ------------------------ */

IncrementalLTReversed_FOS::IncrementalLTReversed_FOS(){
    reinitializeOnNewRound = false;
}

vector<vector<int>> IncrementalLTReversed_FOS::getFOS(int genotypeLength){
    return FOSStructures::getIncrementalLTReversed_FOS(genotypeLength);
}
string IncrementalLTReversed_FOS::id(){ return "incrLT-desc"; }
string IncrementalLTReversed_FOS::toString() { return "Descending Incremental Linkage Tree FOS"; }


/* ------------------------ Incremental LT + Univariate FOS ------------------------ */

IncrementalLT_UnivariateOrdered_FOS::IncrementalLT_UnivariateOrdered_FOS(){
    reinitializeOnNewRound = false;
}

vector<vector<int>> IncrementalLT_UnivariateOrdered_FOS::getFOS (int genotypeLength){
    vector<vector<int>> fos;
    fos.reserve(2 * genotypeLength);
    vector<vector<int>> fixedLT = FOSStructures::getIncrementalLT_FOS(genotypeLength);
    vector<vector<int>> univariate = FOSStructures::getUnivariate_FOS(genotypeLength, Utility::Order::ASCENDING);
    fos.insert(fos.begin(), fixedLT.begin(), fixedLT.end());
    fos.insert(fos.end(), univariate.begin(), univariate.end());
    return fos;
}
string IncrementalLT_UnivariateOrdered_FOS::id() { return "incrLT_Uni-asc"; }
string IncrementalLT_UnivariateOrdered_FOS::toString(){ return "Ascending Incremental Linkage Tree, followed by (ascending) Univariate FOS"; }


/* ------------------------ Incremental LT Reversed + Ordered Univariate FOS ------------------------ */

IncrementalLTReversed_UnivariateOrdered_FOS::IncrementalLTReversed_UnivariateOrdered_FOS(){
    reinitializeOnNewRound = false;
}

vector<vector<int>> IncrementalLTReversed_UnivariateOrdered_FOS::getFOS (int genotypeLength){
    vector<vector<int>> fos;
    fos.reserve(2 * genotypeLength);
    vector<vector<int>> fixedLT = FOSStructures::getIncrementalLTReversed_FOS(genotypeLength);
    vector<vector<int>> univariate = FOSStructures::getUnivariate_FOS(genotypeLength, Utility::Order::ASCENDING);
    fos.insert(fos.begin(), fixedLT.begin(), fixedLT.end());
    fos.insert(fos.end(), univariate.begin(), univariate.end());
    return fos;
}
string IncrementalLTReversed_UnivariateOrdered_FOS::id() { return "incrLT-desc_Uni-asc"; }
string IncrementalLTReversed_UnivariateOrdered_FOS::toString(){ return "Descending Incremental Linkage Tree, followed by (ascending) Univariate FOS"; }


/* ------------------------ Incremental LT Reversed + Univariate FOS ------------------------ */

IncrementalLTReversed_Univariate_FOS::IncrementalLTReversed_Univariate_FOS(){
    reinitializeOnNewRound = true;
}

vector<vector<int>> IncrementalLTReversed_Univariate_FOS::getFOS (int genotypeLength){
    vector<vector<int>> fos;
    fos.reserve(2 * genotypeLength);
    vector<vector<int>> fixedLT = FOSStructures::getIncrementalLTReversed_FOS(genotypeLength);
    vector<vector<int>> univariate = FOSStructures::getUnivariate_FOS(genotypeLength, Utility::Order::RANDOM);
    fos.insert(fos.begin(), fixedLT.begin(), fixedLT.end());
    fos.insert(fos.end(), univariate.begin(), univariate.end());
    return fos;
}
string IncrementalLTReversed_Univariate_FOS::id() { return "incrLT-desc_Uni-rand"; }
string IncrementalLTReversed_Univariate_FOS::toString(){ return "Descending Incremental Linkage Tree, followed by (random) Univariate FOS"; }


/* ------------------------ Unordered Univariate FOS ------------------------ */

Univariate_FOS::Univariate_FOS(Utility::Order order) : order(order){
    if (order == Utility::Order::RANDOM){
        reinitializeOnNewRound = true;
    } else {
        reinitializeOnNewRound = false;
    }
}

vector<vector<int>> Univariate_FOS::getFOS (int genotypeLength){
    return FOSStructures::getUnivariate_FOS(genotypeLength, order);
}

string Univariate_FOS::id() { return "Uni-" + Utility::orderToID(order); }
string Univariate_FOS::toString(){ return Utility::orderToString(order) + " Univariate FOS"; }


/* ------------------------ Triplet FOS ------------------------------------- */

Triplet_FOS::Triplet_FOS(Utility::Order order) : order(order){
    reinitializeOnNewRound = false;
}

vector<vector<int>> Triplet_FOS::getFOS (int genotypeLength){
    return FOSStructures::getTriplet_FOS(genotypeLength, order);
}

string Triplet_FOS::id() { return "Triplet-" + Utility::orderToID(order); }
string Triplet_FOS::toString(){ return Utility::orderToString(order) + " Triplet FOS"; }


/* ------------------------ Triplet Tree FOS ------------------------------------- */

TripletTree_FOS::TripletTree_FOS(Utility::Order order) : order(order){
    reinitializeOnNewRound = false;
}

vector<vector<int>> TripletTree_FOS::getFOS (int genotypeLength){
    return FOSStructures::getTripletTree_FOS(genotypeLength, order);
}

string TripletTree_FOS::id() { return "TripletTree-" + Utility::orderToID(order); }
string TripletTree_FOS::toString(){ return Utility::orderToString(order) + " Triplet Tree FOS"; }


/* ------------------------ ARK6 FOS ------------------------------------- */
// This FOS has subsets that denote the entire block between two pooling layers.
// Pooling is done after 4th, 8th and 12th layer, so that's how the subsets are separated.

ARK6_FOS::ARK6_FOS(Utility::Order order) : order(order){
    reinitializeOnNewRound = false;
}

vector<vector<int>> ARK6_FOS::getFOS (int genotypeLength){
    return FOSStructures::getARK6_FOS(genotypeLength, order);
}

string ARK6_FOS::id() { return "ARK6-" + Utility::orderToID(order); }
string ARK6_FOS::toString(){ return Utility::orderToString(order) + " ARK6 FOS"; }


/* ------------------------ RandomTree FOS ------------------------------------- */

RandomTree_FOS::RandomTree_FOS() {
    reinitializeOnNewRound = true;
}

vector<vector<int>> RandomTree_FOS::getFOS (int genotypeLength){
    vector<vector<int>> result;
    vector<vector<int>> pool;
    for (int i = 0; i < genotypeLength; i++){
        vector<int> temp = {i};
        result.push_back(temp);
        pool.push_back(temp);
    }
    while (pool.size() >= 2){
//    for (int i = 0; i < genotypeLength - 1; i++){
        int rand1 = Utility::getRand(0, pool.size());
        vector<int> element1 = pool[rand1];
        pool.erase(pool.begin() + rand1);
        
        int rand2 = Utility::getRand(0, pool.size());
        vector<int> element2 = pool[rand2];
        pool.erase(pool.begin() + rand2);
        
        vector<int> newElement = mergeElements(element1, element2);
        result.push_back(newElement);
        pool.push_back(newElement);
    }
    result.pop_back(); // Removing the last element containing all variables.
    return result;
}

vector<int> RandomTree_FOS::mergeElements (vector<int> &element1, vector<int> &element2){
    vector<int> result (element1);
    result.insert(result.end(), element2.begin(), element2.end());
    return result;
}

string RandomTree_FOS::id() { return "RT"; }
string RandomTree_FOS::toString(){ return "Random Tree FOS"; }

/* ------------------------ TupleTree FOS ------------------------------------- */
TupleTree_FOS::TupleTree_FOS(Utility::Order order) : order(order){
    reinitializeOnNewRound = true;
}

vector<vector<int>> TupleTree_FOS::getFOS (int genotypeLength){
    return FOSStructures::getTupleTree_FOS(genotypeLength, order);
}

string TupleTree_FOS::id() { return "TT-" + Utility::orderToID(order); }
string TupleTree_FOS::toString(){ return Utility::orderToString(order) + "Tuple Tree FOS"; }


/* ------------------------ Namespace FOS Structures ------------------------ */

vector<vector<int>> FOSStructures::getIncrementalLT_FOS(int n){
    vector<vector<int>> fos;
    fos.reserve(n);
    for(int i = 0; i < n; i++){
        vector<int> subset (i + 1, 0);
        for(int j = 0; j < (i + 1); j++){
            subset[j] = j;
        }
        fos.push_back(subset);
    }
    return fos;
}

vector<vector<int>> FOSStructures::getIncrementalLTReversed_FOS(int n){
    vector<vector<int>> fos;
    fos.reserve(n);
    for(int i = n - 1; i >= 0; i--){
        vector<int> subset (n - i, 0);
        for(int j = n - 1; j >= i; j--){
            subset[n-j-1] = j;
        }
        fos.push_back(subset);
    }
    return fos;
}

vector<vector<int>> FOSStructures::getUnivariate_FOS(int n, Utility::Order order){
    vector<vector<int>> fos;
    fos.reserve(n);
    vector<int> orderedArray = Utility::getOrderedArray(n, order);
    for(int i = 0; i < n; i++){
        vector<int> subset(1, 0);
        subset[0] = orderedArray[i];
        fos.push_back(subset);
    }
    return fos;
}

vector<vector<int>> FOSStructures::getTriplet_FOS(int n, Utility::Order order){
    vector<vector<int>> fos;
    fos.reserve(n/3);
    
    vector<int> orderArray = Utility::getOrderedArray(n, order);
    
    for (int i = 0; i < n/3; i++){
        vector<int> subset(3, 0);
        subset[0] = orderArray[i * 3];
        subset[1] = orderArray[i * 3 + 1];
        subset[2] = orderArray[i * 3 + 2];
        fos.push_back(subset);
    }
    return fos;
}

vector<vector<int>> FOSStructures::getTripletTree_FOS(int n, Utility::Order order){
    vector<vector<int>> fos = getTriplet_FOS(n, order);
    //Easy hardcoded way
    for (int i = 0; i < 2; i++){
        vector<int> subset (6, 0);
        for (int j = 0; j < 2; j++){
            for (int k = 0; k < 3; k++){
                subset[j*3 + k] = fos[i*2 + j][k];
            }
        }
        fos.push_back(subset);
    }
    
    //Complex modular way, not complete
//    int currentSize = fos.size();
//    int count = 10;
//    do {
//        for (int i = 0; i < currentSize/2; i++){
//            vector<int> subset (fos[i].size() + fos[i+1].size());
//        }
//    } while (count >= 2);
    return fos;
}

vector<vector<int>> FOSStructures::getARK6_FOS(int n, Utility::Order order){
    vector<vector<int>> temp;
    temp.reserve(ceil(n / 4.0));
    
    for (int i = 0; i < n; i++){
        if(i % 4 == 0){
            int elementsLeft = min(n - i, 4);
            temp.push_back(vector<int> (elementsLeft));
        }
        temp[floor(i/4)][i % 4] = i;
    }
    
    vector<vector<int>> fos;
    vector<int> orderArray = Utility::getOrderedArray(temp.size(), order);
    
    for (int i : orderArray){
        fos.push_back(temp[i]);
    }

    return fos;
}

vector<vector<int>> FOSStructures::getTuple_FOS(int n, Utility::Order order){
    vector<vector<int>> fos;
    fos.reserve(n/2);
    
    vector<int> orderArray = Utility::getOrderedArray(n, order);
    
    for (int i = 0; i < n/2; i++){
        vector<int> subset(2, 0);
        subset[0] = orderArray[i * 2];
        subset[1] = orderArray[i * 2 + 1];
        fos.push_back(subset);
    }
    return fos;
}

vector<vector<int>> FOSStructures::getTupleTree_FOS(int n, Utility::Order order){
    vector<vector<int>> fos = getTuple_FOS(n, order);
    vector<vector<int>> pool;
    for (int i = 0; i < fos.size(); i++){
        vector<int> temp;
        temp = fos[i];
        pool.push_back(temp);
    }
    while(pool.size() > 2){
        vector<int> orderedArray = Utility::getOrderedArray(pool.size(), order);
        int idx1 = orderedArray[0];
        int idx2 = orderedArray[1];
        vector<int> temp;
        for (int i = 0; i < pool[idx1].size(); i++){
            temp.push_back(pool[idx1][i]);
        }
        for (int i = 0; i < pool[idx2].size(); i++){
            temp.push_back(pool[idx2][i]);
        }
        fos.push_back(temp);
        if (idx1 > idx2){
            pool.erase(pool.begin() + idx1);
            pool.erase(pool.begin() + idx2);
        } else {
            pool.erase(pool.begin() + idx2);
            pool.erase(pool.begin() + idx1);
        }
        pool.push_back(temp);
    }
    return fos;
}

void FOSStructures::printFOS(std::vector<vector<int>> fos){
    cout << "FOS: ";
    for (int i = 0; i < fos.size(); i++){
        // Uncomment below to display only subsets of a certain length
//        if (fos[i].size() != 3)
//            continue;
        cout << "[";
        vector<int> subset = fos[i];
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

vector<vector<int>> FOSStructures::sortFOSMeanAscending (vector<vector<int>> & fos){
    sort( fos.begin(), fos.end(), []( const vector<int>& lhs, const vector<int>& rhs){
        float mean_lhs = 0.0;
        for (int i : lhs)
            mean_lhs += 1.0 * i;
        mean_lhs /= lhs.size();
        
        float mean_rhs = 0.0;
        for (int i : rhs)
            mean_rhs += 1.0 * i;
        mean_rhs /= rhs.size();
        
        return mean_lhs < mean_rhs;
    });
    return fos;
}

vector<vector<int>> FOSStructures::sortFOSMeanDescending (vector<vector<int>> & fos){
    vector<vector<int>> orderedFos = sortFOSMeanAscending(fos);
    reverse(orderedFos.begin(), orderedFos.end());
    return orderedFos;
}

/**
 Bounds the FOS by trimming the layers with < bottomLevel elements and > topLevel elements
 */
vector<vector<int>> FOSStructures::boundFOS (vector<vector<int>> & fos, int bottomLevel, int topLevel){
    vector<vector<int>> boundedFOS;
    
    for (int i = 0; i < fos.size(); i++){
        if(fos[i].size() >= bottomLevel && fos[i].size() <= topLevel){
            boundedFOS.push_back(fos[i]);
        }
    }
    
    return boundedFOS;
}

vector<int> FOSStructures::sortFOSElement(std::vector<int> &element){
    vector<int> result(element);
    sort(result.begin(), result.end());
    return result;
}

string FOSStructures::elementToString(std::vector<int> &element){
    string result = "";
    for (int i = 0; i < element.size(); i++){
        result += to_string(element[i]);
        if (i != element.size()-1){
            result += ",";
        }
    }
    return result;
}
