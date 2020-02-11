//
//  FitnessFunction.hpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef FitnessFunction_hpp
#define FitnessFunction_hpp

#include <stdio.h>
#include <nlohmann/json.hpp>
#include "Individual.hpp"
#include "ProblemType.hpp"
#include "UniqueSolutions.hpp"

class FitnessFunction {
public:
    Individual bestIndividual;
    std::vector<Individual> elitistArchive;
    std::vector<float> optimum;
    int optimalParetoFrontSize;
    bool optimumFound;
    int maxEvaluations;
    int maxUniqueEvaluations;
    ProblemType *problemType;
    int totalProblemLength;
    int numObjectives = 1;
    bool checkForGenotype = false;
    arma::uvec optimalGenotype;
    
    int totalEvaluations;
    int totalUniqueEvaluations;
    int totalTransformedUniqueEvaluations;
    UniqueSolutions uniqueSolutions;
    UniqueSolutions transformedUniqueSolutions;
    
    FitnessFunction(std::vector<float> optimum, ProblemType *problemType);
    FitnessFunction(ProblemType *problemType);
    
    virtual void clear();
    
    virtual std::vector<float> evaluate(Individual &ind) = 0;
    void evaluationProcedure(Individual &ind);
    virtual void display();
    virtual std::string id();
    void setProblemType(ProblemType* problemType);
    
    void checkIfBestFound(Individual &ind);
    void updateElitistArchive(std::vector<Individual*> front);
    void updateElitistArchive(Individual &ind);
    virtual bool entireParetoFrontFound ();
    void setGenotypeChecking(arma::uvec genotype);
    
    int getTotalAmountOfEvaluations();
    bool maxEvaluationsExceeded();
    bool maxUniqueEvaluationsExceeded();
    
    virtual FitnessFunction* clone() const = 0;
    
    virtual void setLength (int length);
    virtual void setNumObjectives (int numObjectives);
    virtual void setOptimum (std::vector<float> optimum);
    virtual void setOptimum (float optimum);
    
    virtual arma::uvec transform(arma::uvec &genotype);
    
    void draw2DVisualization(std::vector<Individual> &population, int maxX, int maxY);
    void drawElitistArchive();
    
    void saveElitistArchiveToJSON();
};

class OneMax : public FitnessFunction {
public:
    OneMax (int length);
    OneMax ();
    std::vector<float> evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    ProblemType* getProblemType();
    FitnessFunction* clone() const override;
};


class LeadingOnes : public FitnessFunction {
public:
    LeadingOnes (int length);
    LeadingOnes ();
    std::vector<float> evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    ProblemType* getProblemType();
    FitnessFunction* clone() const override;
};

class NonBinaryMax : public FitnessFunction {
public:
    NonBinaryMax ();
    std::vector<float> evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    ProblemType* getProblemType();
    FitnessFunction* clone() const override;
    void setLength (int length) override;
};

#endif /* FitnessFunction_hpp */
