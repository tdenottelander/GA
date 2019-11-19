//
//  GA.hpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef GA_hpp
#define GA_hpp

#include "Selection.hpp"
#include "Variation.hpp"
#include "FitnessFunction.hpp"
#include "Individual.hpp"

#include <armadillo>
#include <vector>
#include <string>

class GA {
public:
    int populationSize;
    FitnessFunction * fitFunc;
    Selection * selection;
    Variation * variation;
    std::vector<Individual> population;
    int problemLength;
    
    GA(int popSize, int probLength, FitnessFunction *f, Selection *s, Variation *v);
    
    void initialize();
    
    void round();
    
    void evaluateAll(std::vector<Individual> &population);
    
    double getAvgFitness();
    
    std::string toString();
};

#endif /* GA_hpp */
