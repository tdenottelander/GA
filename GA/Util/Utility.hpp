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
#include <nlohmann/json.hpp>

extern std::mt19937 rng;
extern std::uniform_real_distribution<float> dist;


namespace Utility{
    enum class Order {RANDOM, ASCENDING, DESCENDING};
    std::string orderToID (Order order);
    std::string orderToString (Order order);
    std::vector<int> getOrderedArray (int n, Order order);
    std::vector<int> getRandomlyPermutedArray (int n);
    std::vector<int> getRandomlyPermutedArrayV2 (int n);
    std::vector<int> getAscendingArray (int n);
    std::vector<int> getDescendingArray (int n);
    double getRand();
    int getRand(int start, int end);
    int getConditionalBit(int counter0, int counter1, int max);
    long millis();
    std::string getDateString();
    std::string padFrontWith0(std::string target, int length);
    std::string removeTrailingZeros(std::string target);
    std::string padWithSpacesAfter(std::string target, int length);
    void write(std::string content, std::string dir, std::string filename);
    void writeRawData(std::string content, std::string dir, std::string suffix = "");
    void writeJSON (nlohmann::json content, std::string filename);
    nlohmann::json readJSON(std::string filename);
    void read(std::string filename);
    std::string genotypeToString(arma::uvec &genotype);
    arma::uvec stringToGenotype(std::string &genotype);
    arma::uvec vectorToUvec (std::vector<int> vec);
    std::vector<int> uvecToVector (arma::uvec vec);
}

#endif /* Utility_hpp */
