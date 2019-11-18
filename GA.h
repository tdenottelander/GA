#ifndef GA_H
#define GA_H

#include "Selection.h"
#include "Variation.h"
#include "FitnessFunction.h"
#include "Individual.h"

#include <armadillo>
#include <vector>
#include <string>

class GA {
    public:
        int populationSize;
        FitnessFunction * fitFunc;
        Selection * selection;
        Variation * variation;
        std::vector<Individual> population;
        int problemLength;

        GA(int popSize, int probLength, FitnessFunction *f, Selection *s, Variation *v);

        void initialize();

        void round();

        double getAvgFitness();

        std::string toString();
};

#endif GA_H