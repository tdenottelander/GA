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
#include <unordered_set>
#include "GA.hpp"
#include "NSGA_II.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "Utility.hpp"
#include "UniqueSolutions.hpp"

class RoundSchedule {
public:
    int maxRounds;
    int maxPopSizeLevel;
    int maxSeconds;
    int maxEvaluations;
    int maxUniqueEvaluations;
    int interval;
    Selection* selection;
    Variation* variation;
    GA* ga;
    nlohmann::json output;
    std::vector<GA*> gaList;
    std::vector<int> whichShouldRun;
    RoundSchedule(int maxRounds, int maxPopSizeLevel, int maxSeconds, int maxEvaluations, int maxUniqueEvaluations, int interleavedRoundInterval);
    void initialize(GA *ga, int problemSize, bool nonIMSmode = false, int nonIMSpopsize = 0);
    nlohmann::json run();
    void terminateGAs(int n);
    int getAmountOfEvaluations();
    bool maxEvaluationsExceeded();
    bool maxUniqueEvaluationsExceeded();
    void writeOutputGenerationCSV(std::string filename);
};

#endif /* RoundSchedule_hpp */
