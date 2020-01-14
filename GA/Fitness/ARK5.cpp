//
//  ARK5.cpp
//  GA
//
//  Created by Tom den Ottelander on 14/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK5.hpp"


using namespace std;
using namespace arma;
using namespace nlohmann;

ARK5::ARK5() : ARK(8, true, -1, getProblemType(true), 2, 2, "ark5") {}

ARK5::ARK5(int problemSize, bool allowIdentityLayers, int maxEvaluations) : ARK(problemSize, allowIdentityLayers, maxEvaluations, getProblemType(allowIdentityLayers), 2, 2, "ark5") {}

void ARK5::display(){
    cout << "ARK-5 fitness function" << endl;
}

string ARK5::id(){
    if(allowIdentityLayers)
        return("ARK-5_inclID");
    else
        return("ARK-5_exclID");
}

FitnessFunction* ARK5::clone() const {
    return new ARK5(static_cast<const ARK5&>(*this));
}

ProblemType* ARK5::getProblemType(bool allowIdentityLayers){
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

void ARK5::setLength (int length) {
    totalProblemLength = length;
}
