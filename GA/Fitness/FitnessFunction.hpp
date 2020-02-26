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
#include <tuple>
#include "Individual.hpp"
#include "ProblemType.hpp"
#include "UniqueSolutions.hpp"

class FitnessFunction {
public:
    
    enum class ConvergenceCriteria {OPTIMAL_FITNESS, EPSILON_FITNESS, GENOTYPE, ENTIRE_PARETO, EPSILON_PARETO_DISTANCE, PERCENTAGE_PARETO_COVERAGE, ABSOLUTE_PARETO_COVERAGE};
    
    ConvergenceCriteria convergenceCriteria;
    Individual bestIndividual;
    std::vector<Individual> elitistArchive;
    std::vector<float> optimum;
    int optimalParetoFrontSize;
    std::vector<std::vector<float>> trueParetoFront;
    bool optimumFound;
    int maxEvaluations;
    int maxUniqueEvaluations;
    ProblemType *problemType;
    int totalProblemLength;
    int numObjectives = 1;
    bool checkForGenotype = false;
    arma::uvec optimalGenotype;
    float epsilon = 0.0f;
    bool done = false;
    
    int totalEvaluations;
    int totalUniqueEvaluations;
    int totalTransformedUniqueEvaluations;
    UniqueSolutions uniqueSolutions;
    UniqueSolutions transformedUniqueSolutions;
    
    // Stored as TotalEvaluations, TotalUniqueEvaluations, Distance
    std::vector<std::tuple<int, int, float>> distanceToParetoFrontData;
    std::vector<std::vector<Individual>> elitistArchiveHistory;
    nlohmann::json elitistArchiveHistoryJSON;
    
    FitnessFunction(std::vector<float> optimum, ProblemType *problemType);
    FitnessFunction(ProblemType *problemType);
    
    virtual void clear();
    
    virtual std::vector<float> evaluate(Individual &ind) = 0;
    void evaluationProcedure(Individual &ind);
    bool isDone();
    virtual void display();
    virtual std::string id();
    void setProblemType(ProblemType* problemType);
    
    void checkIfBestFound(Individual &ind);
    bool updateElitistArchive(std::vector<Individual*> front);
    bool updateElitistArchive(Individual &ind);
    virtual bool entireParetoFrontFound ();
    float calculateDistanceParetoToApproximation ();
    nlohmann::json paretoDistanceToJSON ();
    nlohmann::json elitistArchiveHistoryToJSON();
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
