//
//  ARK2.cpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK2.hpp"

using namespace std;
using namespace nlohmann;

ARK2::ARK2() : ARK(13, true, false, getProblemType(true), 2, 4, "ark2") {}

ARK2::ARK2(int problemSize, bool allowIdentityLayers, bool genotypeChecking) : ARK(problemSize, allowIdentityLayers, genotypeChecking, getProblemType(allowIdentityLayers), 2, 4, "ark2") {}

void ARK2::display(){
    cout << "ARK-2 fitness function" << endl;
}

string ARK2::id(){
    if(allowIdentityLayers)
        return("ARK-2_inclID");
    else
        return("ARK-2_exclID");
}

FitnessFunction* ARK2::clone() const {
    return new ARK2(static_cast<const ARK2&>(*this));
}

ProblemType* ARK2::getProblemType(bool allowIdentityLayers){
    // 0 = 3x3 convolution
    // 1 = 2x2 max pooling
    // 2 = identity
    vector<int> alphabet;
    if(allowIdentityLayers)
        alphabet = {0,1,2};
    else
        alphabet = {0,1};
    return new AlphabetProblemType(alphabet);
}
