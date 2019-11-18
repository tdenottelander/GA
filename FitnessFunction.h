#ifndef FITNESSFUNCTION_H
#define FITNESSFUNCTION_H

#include "Individual.h"

class FitnessFunction {
    public:
        Individual * bestIndividual;

        FitnessFunction();

        virtual int evaluate(Individual &ind);
        virtual void display();
};

class OneMax : public FitnessFunction {
    public:
        OneMax ();
        int evaluate(Individual &ind) override;
        void display() override;
};

#endif //FITNESSFUNCTION_H