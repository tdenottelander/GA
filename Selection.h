#ifndef SELECTION_H
#define SELECTION_H

#include <iostream>
#include <vector>
#include "Individual.h"

class Selection {
    public:
        // Selection();
        std::vector<Individual> select(std::vector<Individual> &population, int outputSize);
        virtual void display();
};

class ProportionateSelection : public Selection {
    public:
        ProportionateSelection();
        std::vector<Individual> select(std::vector<Individual> &population, int outputSize);
        void display() override;
        std::vector<float> getProportions(std::vector<Individual> &population);
};

class TournamentSelection : public Selection {
    public:
        std::vector<Individual> select(std::vector<Individual> &population, int outputSize, int tournamentSize);
        void display() override;
};

#endif //SELECTION_H