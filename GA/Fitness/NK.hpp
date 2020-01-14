//
//  NK.hpp
//  GA
//
//  Created by Tom den Ottelander on 14/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef NK_hpp
#define NK_hpp

#include <stdio.h>
#include <unordered_map>
#include "FitnessFunction.hpp"

class NK : public FitnessFunction {
public:
    int blocksize;
    int numBlocks;
    bool wraparound;
    int alphabetSize;
    int tableIndex;
    nlohmann::json table;
//    NK();
    NK(int problemLength, int blocksize, bool wraparound, int alphabetsize, int maxEvaluations);
    void clear() override ;
    void createTable();
    float evaluate(Individual &ind) override;
    float evaluateBlock(int blockIndex, arma::uvec block);
    ProblemType* getProblemType(int alphabetsize);
    
    void loadTable(int tableIndex);
    
    void display() override;
    std::string id() override;
    static std::string getFileName(int blocksize, int wraparound, int alphabetSize, int amountOfTables);
    FitnessFunction* clone() const override;
    void setLength (int length) override;
    
    static void generateTables(int problemLength, int blocksize, bool wraparound, int alphabetSize, int amountOfTables);
    static float getOptimum(nlohmann::json table, int problemLength, int blocksize, bool wraparound, int alphabetSize);
};

static std::string folderprefix = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/NK/";

#endif /* NK_hpp */
