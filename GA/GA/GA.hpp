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
    bool terminated;
    bool initialized;
    int roundsCount;
    
    GA(FitnessFunction * fitfunc);
    
    virtual void initialize();
    
    void run();
    virtual void round();
    
    int findMinimallyNeededPopulationSize(int repetitions, int successesNeeded);
    
    void roundReplacementVariationSelection();
    void roundPOVariationSelection();
    
    void evaluateAll(std::vector<Individual> &population);
    void evaluateAll();
    bool isConverged();
    bool isDiverse();
    bool isOptimal();
    std::vector<float> getAvgFitness();
    int getTotalAmountOfEvaluations();
    
    void initializeTrueRandomPopulation();
    void initializeSolvablePopulation();
    void initializeUninitializedPopulation();
    
    std::string toString();
    std::string populationToString(std::vector<Individual> &population);
    void print();
    
    void setPopulationSize (int n);
    void setFitnessFunction (FitnessFunction * fitfunc);
    void setSelection (Selection * sel);
    void setVariation (Variation * var);
    void setPointers(FitnessFunction * fitfunc, Selection * sel, Variation * var);
    
    virtual GA* clone() const;
    
    virtual std::string id();
};

#endif /* GA_hpp */
