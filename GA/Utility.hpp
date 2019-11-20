//
//  Utility.hpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp

#include <stdio.h>
#include <vector>
#include <armadillo>

extern std::mt19937 rng;
extern std::uniform_real_distribution<float> dist;

namespace Utility{
    std::vector<int> getRandomlyPermutedArray (int n);
    std::vector<int> getRandomlyPermutedArrayV2 (int n);
    double getRand();
}

#endif /* Utility_hpp */
