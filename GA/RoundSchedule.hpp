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
    int maxLevel;
    int interval;
    Selection* selection;
    Variation* variation;
    nlohmann::json output;
    std::vector<GA*> gaList;
    std::vector<int> whichShouldRun;
    RoundSchedule(int maxRounds, int maxLevel);
    void initialize(Selection &selection, Variation &variation, int problemSize);
    nlohmann::json run();
    void terminateGAs(int n);
};

#endif /* RoundSchedule_hpp */
