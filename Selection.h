#pragma once

#include <vector>
#include "Individual.h"

using namespace std;
using namespace N;

class Selection {
    public:
        void select();
};

class ProportionateSelection : public Selection {
    public:
        vector<Individual> select (vector<Individual> &population, int outputSize);
        vector<float> getProportions (vector<Individual> &population);
};

class TournamentSelection : public Selection {
    void select(vector<Individual> &population, int outputSize, int tournamentSize);
};
