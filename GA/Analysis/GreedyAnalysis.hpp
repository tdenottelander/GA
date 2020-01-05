//
//  GreedyAnalysis.hpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef GreedyAnalysis_hpp
#define GreedyAnalysis_hpp

#include <stdio.h>
#include <vector>
#include "ARK.hpp"
#include "ARK1.hpp"
#include "ARK2.hpp"

namespace GreedyAnalysis {
    void greedyRun();
    void greedyRunBackward();
    void greedyBothWays();
    void greedyInsideOut();
    void findBest(int length, int alphabetSize);
    void findBestRecursion(int length, int alphabetSize, std::vector<int> &temp, int idx);
}

static float bestSoFar = -1.0;
//static std::string defaultFolder = "ark1";

#endif /* GreedyAnalysis_hpp */
