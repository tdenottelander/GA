//
//  RoundSchedule.hpp
//  GA
//
//  Created by Tom den Ottelander on 22/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef RoundSchedule_hpp
#define RoundSchedule_hpp

#include <stdio.h>
#include <vector>
#include <nlohmann/json.hpp>
#include "GA.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "Utility.hpp"

class RoundSchedule {
public:
    int maxRounds;
    int maxPopSizeLevel;
    int maxSeconds;
    int maxEvaluations;
    int interval;
    Selection* selection;
    Variation* variation;
    GA* ga;
    nlohmann::json output;
    std::vector<GA*> gaList;
    std::vector<int> whichShouldRun;
    Individual bestIndividualOverall;
    RoundSchedule(int maxRounds, int maxPopSizeLevel, int maxSeconds, int maxEvaluations, int interleavedRoundInterval);
    void initialize(GA *ga, int problemSize);
    nlohmann::json run();
    void terminateGAs(int n);
    int getAmountOfEvaluations();
};

#endif /* RoundSchedule_hpp */
