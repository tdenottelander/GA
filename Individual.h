#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <armadillo>
#include <string>

class Individual {
    public:
        arma::uvec genotype;
        int fitness;

        Individual(int length);
        void initialize();
        Individual copy();
        std::string toString();
};

#endif //INDIVIDUAL_H