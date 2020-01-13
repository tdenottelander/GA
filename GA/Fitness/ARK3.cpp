//
//  ARK3.cpp
//  GA
//
//  Created by Tom den Ottelander on 07/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK3.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

ARK3::ARK3() : ARK(12, false, -1, ark3_optimum, getProblemType(), -1, 4, "ark3") {}

ARK3::ARK3(int maxEvaluations) : ARK(12, false, maxEvaluations, ark3_optimum, getProblemType(), -1, 4, "ark3") {}

void ARK3::display(){
    cout << "ARK-3 fitness function" << endl;
}

string ARK3::id(){
    if(allowIdentityLayers)
        return("ARK-3_inclID");
    else
        return("ARK-3_exclID");
}

FitnessFunction* ARK3::clone() const {
    return new ARK3(static_cast<const ARK3&>(*this));
}

ProblemType* ARK3::getProblemType(){
    // 0 = 3x3 convolution
    // 1 = 2x2 max pooling
    return new AlphabetProblemType({0,1});
}

void ARK3::setLength (int length) {
    totalProblemLength = length;
}

uvec ARK3::transform(uvec &genotype){
    return removeIdentities(genotype, identityLayer);
}
