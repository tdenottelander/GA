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
    NSGA_II(FitnessFunction * fitFunc, Variation * var, int tournamentSize, float crossoverProbability, bool mutation, bool visualize = false);
    int tournamentSize;
    float crossoverProbability;
    bool mutation;
    bool visualize = false;
    void round() override;
        
    void clearMOinformation(std::vector<Individual> &population);
    std::vector<std::vector<Individual*>> nonDominatedSorting (std::vector<Individual> &population, int n = -1);
    void CrowdingDistanceSorting (std::vector<Individual*> &front);
    bool crowdComparisonOperator(const Individual* lhs, const Individual* rhs);
    std::vector<Individual*> selectPt (std::vector<std::vector<Individual*>> sortedCandidates);
    std::vector<Individual> createOffspring(std::vector<Individual*> Pt);
    void mutate(Individual &ind, float probability);
    
    bool isDiverse() override;
    GA* clone() const override;
    std::string id() override;
    
    void draw2DVisualization(std::vector<Individual> &population, int maxX, int maxY);
    
    struct CandidateObjectiveComparator
    {
        CandidateObjectiveComparator(int idx) { this->idx = idx; }
        bool operator()(const Individual* lhs, const Individual* rhs ) const {
            return lhs->fitness[idx] < rhs->fitness[idx];
        }
        int idx;
    };
};

#endif /* NSGA_II_hpp */
