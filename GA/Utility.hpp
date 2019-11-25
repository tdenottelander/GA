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
#include <chrono>
#include <string>

extern std::mt19937 rng;
extern std::uniform_real_distribution<float> dist;

namespace Utility{
    std::vector<int> getRandomlyPermutedArray (int n);
    std::vector<int> getRandomlyPermutedArrayV2 (int n);
    double getRand();
    int getRand(int start, int end);
    long millis();
    std::string getDateString();
    std::string padFrontWith0(std::string target, int length);
    void write(std::string content, std::string filename, std::string suffix = "");
    void read(std::string filename);
}

#endif /* Utility_hpp */
