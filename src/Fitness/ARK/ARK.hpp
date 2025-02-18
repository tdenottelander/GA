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
#include <nlohmann/json.hpp>
#include "FitnessFunction.hpp"
#include <unordered_map>


class ARK : public FitnessFunction {
public:
    bool allowIdentityLayers;
    int identityLayer;
    int jsonAccuracyIndex;
    std::string folder;
    ARK(int problemSize, bool allowIdentityLayers, bool genotypeChecking, ProblemType* problemType, int identity, int jsonAccuracyIndex, std::string folder);
    std::vector<float> evaluate(Individual &ind) override;
    std::vector<float> query(std::vector<int> encoding);
    virtual std::vector<float> getFitness(std::vector<int> encoding);
    int getNumParams(std::vector<int> encoding);
    void display() override;
    std::string id() override;
    void setLength (int length) override;
    float getOptimum(int problemLength);
    
    
    static void printArchitecture(std::vector<int> architecture);
    
    std::vector<int> transform(std::vector<int> &genotype) override;
    
    struct solution {
        float fitness;
        std::vector<std::vector<int>> genotypes;
        int optCount;
        int totalCount;
    };
    
    solution findBest();
    void findBestRecursion(int length, int alphabetSize, std::vector<int> &temp, int idx, solution &statistics);
    std::pair<int,int> findAmountOfArchitecturesWithFitnessAboveThreshold(float threshold);
    
    void doAnalysis(int minLayerSize, int maxLayerSize);
    
    float getOptimum(std::string folder, int layers, bool allowIdentityLayers);
    std::vector<int> getOptimalGenotype();
    void setGenotypeChecking();
    int findMostDifferentGenotype(std::vector<std::vector<int>> &genotypes);
};

static nlohmann::json lookupTable;

#endif /* ARK_hpp */
