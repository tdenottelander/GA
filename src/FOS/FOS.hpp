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
#include "Utility.hpp"
#include "Individual.hpp"

class FOS {
public:
    bool reinitializeOnNewRound;
    virtual std::vector<std::vector<int>> getFOS (std::vector<Individual> &population);
    virtual std::vector<std::vector<int>> getFOS (int genotypeLength) = 0;
    virtual std::string id();
    virtual std::string toString();
};

class IncrementalLT_FOS : public FOS {
public:
    IncrementalLT_FOS();
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLTReversed_FOS : public FOS {
public:
    IncrementalLTReversed_FOS();
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLT_UnivariateOrdered_FOS : public FOS {
public:
    IncrementalLT_UnivariateOrdered_FOS();
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLTReversed_UnivariateOrdered_FOS : public FOS {
public:
    IncrementalLTReversed_UnivariateOrdered_FOS();
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class IncrementalLTReversed_Univariate_FOS : public FOS {
public:
    IncrementalLTReversed_Univariate_FOS();
    std::vector<std::vector<int>> getFOS (int genotypeLengthn) override;
    std::string id() override;
    std::string toString() override;
};

class Univariate_FOS : public FOS {
public:
    Univariate_FOS(Utility::Order order);
    Utility::Order order;
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class Triplet_FOS : public FOS {
public:
    Triplet_FOS(Utility::Order order);
    Utility::Order order;
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class TripletTree_FOS : public FOS {
public:
    TripletTree_FOS(Utility::Order order = Utility::Order::ASCENDING);
    Utility::Order order;
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class ARK6_FOS : public FOS {
public:
    ARK6_FOS(Utility::Order order);
    Utility::Order order;
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

class RandomTree_FOS : public FOS {
public:
    RandomTree_FOS();
    std::vector<std::vector<int>> getFOS (int genotypeLength) override;
    std::vector<int> mergeElements (std::vector<int> &element1, std::vector<int> &element2);
    std::string id() override;
    std::string toString() override;
};

class TupleTree_FOS : public FOS {
public:
    TupleTree_FOS(Utility::Order order);
    Utility::Order order;
    std::vector<std::vector<int>> getFOS(int genotypeLength) override;
    std::string id() override;
    std::string toString() override;
};

namespace FOSStructures {
    std::vector<std::vector<int>> getIncrementalLT_FOS (int n);
    std::vector<std::vector<int>> getIncrementalLTReversed_FOS (int n);
    std::vector<std::vector<int>> getUnivariate_FOS (int n, Utility::Order order);
    std::vector<std::vector<int>> getTriplet_FOS (int n, Utility::Order order);
    std::vector<std::vector<int>> getTripletTree_FOS (int n, Utility::Order order);
    std::vector<std::vector<int>> getARK6_FOS (int n, Utility::Order order);
    std::vector<std::vector<int>> getTuple_FOS (int n, Utility::Order order);
    std::vector<std::vector<int>> getTupleTree_FOS (int n, Utility::Order order);
    void printFOS(std::vector<std::vector<int>> fos);
    std::vector<std::vector<int>> sortFOSMeanAscending (std::vector<std::vector<int>> & fos);
    std::vector<std::vector<int>> sortFOSMeanDescending (std::vector<std::vector<int>> & fos);
    std::vector<std::vector<int>> boundFOS (std::vector<std::vector<int>> & fos, int bottomLevel, int topLevel);
    std::vector<int> sortFOSElement (std::vector<int> &element);
    std::string elementToString (std::vector<int> &element);
};


#endif /* FOS_hpp */
