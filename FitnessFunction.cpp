#pragma once
#include "FitnessFunction.h"

using namespace N;

class FitnessFunction {
    public:
        Individual bestIndividual = (0);

        FitnessFunction(){
            
        }

        int evaluate(Individual &ind){
            int result = 0;
            for(int i = 0; i < ind.genotype.size(); i++){
                result += ind.genotype[i];
            }
            if(result > bestIndividual.fitness){
                bestIndividual = ind;
            }
            return result;
        }
};