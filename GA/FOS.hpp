//
//  FOS.hpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef FOS_hpp
#define FOS_hpp

#include <stdio.h>
#include <vector>
#include <armadillo>
#include "Utility.hpp"
#include "Individual.hpp"

class FOS {
public:
    bool reinitializeOnNewRound;
    virtual std::vector<arma::uvec> getFOS (std::vector<Individual> &population) = 0;
    virtual std::string id();
    virtual std::string toString();
};

class IncrementalLT_FOS : public FOS {
public:
    IncrementalLT_FOS();
    std::vector<arma::uvec> getFOS (std::vector<Individual> &population) override;
    std::string id() override;
    std::string toString() override;
};

class UnivariateOrdered_FOS : public FOS {
public:
    UnivariateOrdered_FOS();
    std::vector<arma::uvec> getFOS (std::vector<Individual> &population) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLT_Univariate_FOS : public FOS {
public:
    IncrementalLT_Univariate_FOS();
    std::vector<arma::uvec> getFOS (std::vector<Individual> &population) override;
    std::string id() override;
    std::string toString() override;
};

class Univariate_FOS : public FOS {
public:
    Univariate_FOS();
    std::vector<arma::uvec> getFOS (std::vector<Individual> &population) override;
    std::string id() override;
    std::string toString() override;
};

namespace FOSStructures {
    std::vector<arma::uvec> getIncrementalLT_FOS (int n);
    std::vector<arma::uvec> getUnivariate_FOS (int n);
    std::vector<arma::uvec> getRandomUnivariate_FOS (int n);
    void printFOS(std::vector<arma::uvec> fos);
};

#endif /* FOS_hpp */
