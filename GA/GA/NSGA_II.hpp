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
    NSGA_II(FitnessFunction * fitFunc, Variation * var, Selection * sel, bool visualize = false);
    bool visualize = false;
    void round() override;
    
    struct Candidate {
        Individual* ind;
        int dominationCount;
        std::vector<Candidate*> dominationList;
        int front;
        float crowdingDistance;
        bool canReproduce;
        std::string toString();
        void print();
    };
    
    std::vector<Candidate> createCandidateStructure (std::vector<Individual> &population);
    std::vector<std::vector<Candidate*>> nonDominatedSorting (std::vector<Candidate> &candidates, int n = -1);
    bool dominates(Individual *ind1, Individual *ind2);
    void CrowdingDistanceSorting (std::vector<Candidate*> &front);
    bool crowdComparisonOperator(const Candidate* lhs, const Candidate* rhs);
    std::vector<Candidate*> selectPt (std::vector<std::vector<Candidate*>> sortedCandidates);
    std::vector<Individual> createOffspring(std::vector<NSGA_II::Candidate*> Pt);
    
    
    GA* clone() const override;
    std::string id() override;
    
    void draw2DVisualization(std::vector<Candidate> &candidates, int maxX, int maxY);
    
    struct CandidateObjectiveComparator
    {
        CandidateObjectiveComparator(int idx) { this->idx = idx; }
        bool operator()(const Candidate* lhs, const Candidate* rhs ) const {
            return lhs->ind->fitness[idx] < rhs->ind->fitness[idx];
        }
        int idx;
    };
};

#endif /* NSGA_II_hpp */
