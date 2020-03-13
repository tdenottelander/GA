//
//  MO_LS.hpp
//  GA
//
//  Created by Tom den Ottelander on 07/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef MO_LS_hpp
#define MO_LS_hpp

#include <stdio.h>
#include <queue>
#include "GA.hpp"
#include "Utility.hpp"

//class LS_Archive {
//public:
//    LS_Archive(bool combineEntries);
//    bool combineEntries;
//    std::vector<std::pair<std::pair<float, float>, std::vector<float>>> archive;
//    
//    bool contains(std::vector<float> objectiveValues);
//    void put(std::pair<std::pair<float, float>, std::vector<float>> pair);
//    void put(std::pair<float, float> scalarization, std::vector<float> objectiveValues);
//    
//    std::pair<float, float> getScalarization (std::vector<float> objectiveValues);
//    std::vector<float> getObjectivevalues (std::pair<float, float> scalarization);
//};


class MO_LS : public GA {
public:
    enum class NewScalarization {RANDOM, SCALARIZATIONSPACE, OBJECTIVESPACE};
    MO_LS(FitnessFunction * fitfunc, Utility::Order order, bool loop, NewScalarization newScalarization);
    Utility::Order LS_order;
    bool loop;
    NewScalarization newScalarization;
    
    std::queue<float> scalarizationTargets;
    std::vector<std::pair<std::pair<float, float>, std::vector<float>>> LS_archive;
    
    void round() override;
    void updateLSArchive(std::pair<float, float> scalarization, std::vector<float> objectiveValues);
    bool equalObjectiveValues(std::vector<float> &o1, std::vector<float> &o2);
    void performLocalSearch(Individual &ind, std::vector<float> scalarization);
    float getNewScalarizationTarget();
    
    GA* clone() const override;
    std::string id() override;
};

#endif /* MO_LS_hpp */
