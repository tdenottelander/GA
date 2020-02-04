//
//  SchaffersStudy.cpp
//  GA
//
//  Created by Tom den Ottelander on 04/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "SchaffersStudy.hpp"

using namespace std;

SchaffersStudy::SchaffersStudy () : FitnessFunction(new BinaryProblemType()) {
    setLength(10);
    setNumObjectives(2);
}

vector<float> SchaffersStudy::evaluate(Individual &ind) {
    vector<float> result (numObjectives, 0);
    int decimal = 0;
    for (int i = 0; i < totalProblemLength; i++){
        decimal += ind.genotype[i] * pow(2, totalProblemLength-i-1);
    }
    result[0] = pow(decimal, 2);
    result[1] = pow(decimal - 2, 2);
    
    ind.fitness = result;
    
    return result;
}

FitnessFunction* SchaffersStudy::clone() const {
    return new SchaffersStudy(static_cast<const SchaffersStudy&>(*this));
}

