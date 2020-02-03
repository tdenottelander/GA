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
    float optimum;
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
    
    FitnessFunction(float optimum, ProblemType *problemType);
    FitnessFunction(ProblemType *problemType);
    
    virtual void clear();
    
    virtual float evaluate(Individual &ind) = 0;
    void evaluationProcedure(Individual &ind);
    virtual void display();
    virtual std::string id();
    void setProblemType(ProblemType* problemType);
    
    void checkIfBestFound(Individual &ind);
    void setGenotypeChecking(arma::uvec genotype);
    
    int getTotalAmountOfEvaluations();
    bool maxEvaluationsExceeded();
    bool maxUniqueEvaluationsExceeded();
    
    virtual FitnessFunction* clone() const = 0;
    
    virtual void setLength (int length);
    virtual void setNumObjectives (int numObjectives);
    
    virtual arma::uvec transform(arma::uvec &genotype);
};

class OneMax : public FitnessFunction {
public:
    OneMax (int length);
    OneMax ();
    float evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    ProblemType* getProblemType();
    FitnessFunction* clone() const override;
};


class LeadingOnes : public FitnessFunction {
public:
    LeadingOnes (int length);
    LeadingOnes ();
    float evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    ProblemType* getProblemType();
    FitnessFunction* clone() const override;
};

class NonBinaryMax : public FitnessFunction {
public:
    NonBinaryMax ();
    float evaluate(Individual &ind) override;
    void display() override;
    std::string id() override;
    ProblemType* getProblemType();
    FitnessFunction* clone() const override;
    void setLength (int length) override;
};

#endif /* FitnessFunction_hpp */
