//
//  ARK.hpp
//  GA
//
//  Created by Tom den Ottelander on 16/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef ARK_hpp
#define ARK_hpp

#include <stdio.h>
#include <vector>
#include <armadillo>
#include <nlohmann/json.hpp>
#include "FitnessFunction.hpp"
#include <unordered_map>


class ARK : public FitnessFunction {
public:
    bool allowIdentityLayers;
    int identityLayer;
    int jsonAccuracyIndex;
    std::string folder;
    ARK(int problemSize, bool allowIdentityLayers, int maxEvaluations, ProblemType* problemType, int identity, int jsonAccuracyIndex, std::string folder);
    float evaluate(Individual &ind) override;
    float query(arma::uvec encoding);
    float query(std::vector<int> encoding);
    int getNumParams(std::vector<int> encoding);
    void display() override;
    std::string id() override;
    void setLength (int length) override;
    float getOptimum(int problemLength);
    
    static void printArchitecture(std::vector<int> architecture);
    
    arma::uvec removeIdentities (arma::uvec &genotype, int identityLayerIndex);
    arma::uvec transform(arma::uvec &genotype) override;
    
    std::pair<float, std::vector<int>> findBest();
    std::pair<float, std::vector<int>> findBestRecursion(int length, int alphabetSize, std::vector<int> &temp, int idx, float bestSoFar);
    std::pair<int,int> findAmountOfArchitecturesWithFitnessAboveThreshold(float threshold);
    
    void doAnalysis(int minLayerSize, int maxLayerSize);
    
    float getOptimum(std::string folder, int layers, bool allowIdentityLayers);
};

static std::string folderPrefix = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/";

#endif /* ARK_hpp */
