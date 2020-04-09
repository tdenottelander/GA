//
//  MO_RS.cpp
//  GA
//
//  Created by Tom den Ottelander on 26/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "MO_RS.hpp"

using namespace std;

MO_RS::MO_RS (FitnessFunction * fitFunc, bool RSUSS) : GA(fitFunc), RSUSS(RSUSS){
    preventIMS = true;
    isRandomSearchAlgorithm = true;
}

void MO_RS::round(){
    Individual &ind = population[0];
    
    if (RSUSS){
        RSUSSInitialize(ind);
    } else {
        ind.initialize(fitFunc_ptr->problemType->alphabet);
    }
    fitFunc_ptr->evaluate(ind);
    fitFunc_ptr->updateElitistArchive(&ind);
    
    roundsCount++;
}

void MO_RS::RSUSSInitialize(Individual &ind){
    int layersToFill = Utility::getRand(1, fitFunc_ptr->totalProblemLength + 1);
    vector<int> shuffledIndices = Utility::getRandomlyPermutedArrayV2(fitFunc_ptr->totalProblemLength);
    for (int i = 0; i < layersToFill; i++){
        ind.genotype[shuffledIndices[i]] = Utility::getRand(1, fitFunc_ptr->problemType->alphabet.size());
    }
    for (int i = layersToFill; i < fitFunc_ptr->totalProblemLength; i++){
        ind.genotype[shuffledIndices[i]] = 0;
    }
}

GA* MO_RS::clone() const {
    return new MO_RS(static_cast<const MO_RS&>(*this));
}

string MO_RS::id() {
    if (RSUSS){
        return "RSUSS";
    } else {
        return "MO-RS";
    }
}
