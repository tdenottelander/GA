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
#include "ARK3.hpp"
#include "ARK4.hpp"

namespace GreedyAnalysis {
    void greedyRun();
    void greedyRunBackward();
    void greedyBothWays();
    void greedyInsideOut();
}

#endif /* GreedyAnalysis_hpp */
