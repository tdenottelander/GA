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

namespace Utility{
    std::vector<int> getRandomlyPermutedArray (int n);
    std::vector<int> getRandomlyPermutedArrayV2 (int n);
    double getRand();
}

#endif /* Utility_hpp */
