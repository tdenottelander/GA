#ifndef VARIATION_H
#define VARIATION_H

#include <iostream>
#include <vector>
#include "Individual.h"

class Variation{
    public:
        // Variation();
        void variate();
        virtual void display();
};

class UnivariateCrossover : public Variation {
    public:
        UnivariateCrossover();
        std::vector<Individual> variate(std::vector<Individual> &population);
        std::vector<Individual> univariateCrossover(Individual &ind1, Individual &ind2);
        void display() override;
};

#endif //VARIATION_H