//
//  Utility.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "Utility.hpp"

using namespace std;

vector<int> Utility::getRandomlyPermutedArray (int n){
    vector<int> arr;
    arr.reserve(n);
    for (int i = 0; i < n; i++) arr.push_back(i);
    shuffle(arr.begin(), arr.end(), default_random_engine());
    return arr;
}

vector<int> Utility::getRandomlyPermutedArrayV2 (int n){
    vector<int> arr;
    arr.reserve(n);
    for (int i = 0; i < n; i++) arr.push_back(i);
    
    vector<int> result;
    result.reserve(n);
    for (int i = 0; i < n; i++){
        float rand = getRand();
        int idx = floor(rand * arr.size());
        result.push_back(arr[idx]);
        arr.erase(arr.begin()+idx);
    }
    
    return result;
}

double Utility::getRand(){
    return ((float)rand() / RAND_MAX);
}
