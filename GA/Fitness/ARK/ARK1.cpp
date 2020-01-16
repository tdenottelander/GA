//
//  ARK1.cpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK1.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

ARK1::ARK1() : ARK(7, true, -1, getProblemType(true), 3, 7, "ark1") {}

ARK1::ARK1(int problemSize, bool allowIdentityLayers, int maxEvaluations) : ARK(problemSize, allowIdentityLayers, maxEvaluations, getProblemType(allowIdentityLayers), 3, 7, "ark1") {}

void ARK1::display(){
    std::cout << "ARK-1 fitness function" << std::endl;
}

std::string ARK1::id(){
    if(allowIdentityLayers)
        return("ARK-1_inclID");
    else
        return("ARK-1_exclID");
}

FitnessFunction* ARK1::clone() const {
    return new ARK1(static_cast<const ARK1&>(*this));
}

ProblemType* ARK1::getProblemType(bool allowIdentityLayers){
    // 0 = 3x3 convolution
    // 1 = 3x3 convolution with stride 2
    // 2 = 5x5 convolution
    // 3 = identity
    vector<int> alphabet;
    if(allowIdentityLayers)
        alphabet = {0,1,2,3};
    else
        alphabet = {0,1,2};
    return new AlphabetProblemType(alphabet);
}

void ARK1::setLength (int length) {
    totalProblemLength = length;
}
