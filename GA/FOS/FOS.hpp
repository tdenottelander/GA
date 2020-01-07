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

class IncrementalLTReversed_FOS : public FOS {
public:
    IncrementalLTReversed_FOS();
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

class UnivariateOrderedReversed_FOS : public FOS {
public:
    UnivariateOrderedReversed_FOS();
    std::vector<arma::uvec> getFOS (std::vector<Individual> &population) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLT_UnivariateOrdered_FOS : public FOS {
public:
    IncrementalLT_UnivariateOrdered_FOS();
    std::vector<arma::uvec> getFOS (std::vector<Individual> &population) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLTReversed_UnivariateOrdered_FOS : public FOS {
public:
    IncrementalLTReversed_UnivariateOrdered_FOS();
    std::vector<arma::uvec> getFOS (std::vector<Individual> &population) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLTReversed_Univariate_FOS : public FOS {
public:
    IncrementalLTReversed_Univariate_FOS();
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
    std::vector<arma::uvec> getIncrementalLTReversed_FOS (int n);
    std::vector<arma::uvec> getOrderedUnivariate_FOS (int n);
    std::vector<arma::uvec> getOrderedUnivariateReversed_FOS (int n);
    std::vector<arma::uvec> getRandomUnivariate_FOS (int n);
    void printFOS(std::vector<arma::uvec> fos);
    std::vector<arma::uvec> sortFOSMeanAscending (std::vector<arma::uvec> & fos);
    std::vector<arma::uvec> sortFOSMeanDescending (std::vector<arma::uvec> & fos);
    std::vector<arma::uvec> boundFOS (std::vector<arma::uvec> & fos, int bottomLevel, int topLevel);
};


#endif /* FOS_hpp */
