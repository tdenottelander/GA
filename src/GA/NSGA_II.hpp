//
//  NSGA_II.hpp
//  GA
//
//  Created by Tom den Ottelander on 03/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef NSGA_II_hpp
#define NSGA_II_hpp

#include <stdio.h>
#include <list>
#include "SimpleGA.hpp"

class NSGA_II : public SimpleGA {
public:
    NSGA_II(FitnessFunction * fitFunc);
    NSGA_II(FitnessFunction * fitFunc, Variation * var, float crossoverProbability, bool mutation, bool visualize = false);
    float crossoverProbability;
    bool doMutation;
    bool visualize = false;
    void round() override;
    
    std::vector<Individual> parentPop;
        
    void clearMOinformation(std::vector<Individual> &population);
    std::vector<std::vector<Individual*>> nonDominatedSorting (std::vector<Individual> &population, int n = -1);
    void CrowdingDistanceSorting (std::vector<Individual*> &front);
    void insertionSort(std::vector<Individual*> &front, int objectiveIdx);
    std::vector<int> quickSort(std::vector<Individual*> &front, int objectiveIdx);
    void quickSort(std::vector<Individual*> &front, std::vector<int> &indices, int objectiveIdx, int left, int right);
    bool crowdComparisonOperator(const Individual &lhs, const Individual &rhs);
    Individual tournament (Individual &ind1, Individual &ind2);
    std::vector<Individual> truncate (std::vector<std::vector<Individual*>> sortedCandidates);
    std::vector<Individual> selection(std::vector<Individual> parentPop);
    void mutation(std::vector<Individual> &children);
    std::vector<Individual> merge (std::vector<Individual> &parentPop, std::vector<Individual> &childPop);
    
    
    bool isDiverse() override;
    GA* clone() const override;
    std::string id() override;
    
    void draw2DVisualization(std::vector<Individual> &population, int maxX, int maxY);
    
    // Candidate Objective Comparator is not used anymore. Quicksort is used instead
    struct CandidateObjectiveComparator
    {
        CandidateObjectiveComparator(int idx) { this->idx = idx; }
        bool operator()(const Individual* lhs, const Individual* rhs ) const {
            float fitlhs = lhs->fitness[idx];
            float fitrhs = rhs->fitness[idx];
            return fitlhs < fitrhs;
            
            // THIS LINE BELOW IS THE RIGHT ONE TO USE, BUT IT BREAKS THE SORT FUNCTION FOR POPULATIONS WITH >4 INDIVIDUALS
//            return fitlhs <= fitrhs;
        }
        int idx;
    };
};

#endif /* NSGA_II_hpp */
