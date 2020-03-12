//
//  ARK4.cpp
//  GA
//
//  Created by Tom den Ottelander on 13/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK4.hpp"

using namespace std;
using namespace nlohmann;

ARK4::ARK4() : ARK(10, true, false, getProblemType(true), 2, 4, "ark4") {}

ARK4::ARK4(int problemSize, bool allowIdentityLayers) : ARK(problemSize, allowIdentityLayers, false, getProblemType(allowIdentityLayers), 2, 4, "ark4") {}

void ARK4::display(){
    cout << "ARK-4 fitness function" << endl;
}

string ARK4::id(){
    if(allowIdentityLayers)
        return("ARK-4_inclID");
    else
        return("ARK-4_exclID");
}

FitnessFunction* ARK4::clone() const {
    return new ARK4(static_cast<const ARK4&>(*this));
}

ProblemType* ARK4::getProblemType(bool allowIdentityLayers){
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
