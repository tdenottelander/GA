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
    FitnessFunction * fitFunc_ptr;
    Selection * selection_ptr;
    Variation * variation_ptr;
    std::vector<Individual> population;
    int problemLength;
    bool terminated;
    bool initialized;
    int roundsCount;
    
    GA(int popSize, int probLength, FitnessFunction *f, Selection *s, Variation *v);
    
    void initialize();
    
    void round();
    
    void roundReplacementVariationSelection();
    void roundPOVariationSelection();
    
    void evaluateAll(std::vector<Individual> &population);
    
    bool isConverged();
    bool isDiverse();
    bool isOptimal();
    
    double getAvgFitness();
    int getTotalAmountOfEvaluations();
    
    std::string toString();
    std::string populationToString(std::vector<Individual> &population);
};

#endif /* GA_hpp */
