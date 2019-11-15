#include "Individual.h"

using namespace N;

class FitnessFunction {
    public:
        Individual bestIndividual;
        FitnessFunction();
        int evaluate(Individual &ind);
};