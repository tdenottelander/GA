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
    NSGA_II(FitnessFunction * fitFunc, Variation * var, Selection * sel);
    void round() override;
    
    struct Candidate {
        Individual* ind;
        int dominationCount;
        std::vector<Candidate*> dominationList;
        int front;
        float crowdingDistance;
        bool canReproduce;
    };
    std::vector<Candidate> candidates;
    std::vector<std::vector<Candidate*>> sortedCandidates;
    
    void createCandidateStructure ();
    void nonDominatedSorting ();
    int dominates(Individual *ind1, Individual *ind2);
    void CrowdingDistanceSorting (std::vector<Candidate*> &front);
    
    struct CrowdingComparator
    {
        CrowdingComparator(int idx) { this->idx = idx; }
        bool operator()(const Candidate* lhs, const Candidate* rhs ) const {
            return lhs->ind->fitness[idx] < rhs->ind->fitness[idx];
        }
        int idx;
    };
    
    GA* clone() const override;
    std::string id() override;
    
    void draw2DVisualization(int maxX, int maxY);
};

#endif /* NSGA_II_hpp */
