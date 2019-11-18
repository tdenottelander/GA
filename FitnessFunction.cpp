#include "FitnessFunction.h"

using namespace std;

/* ------------------------ Base Fitness Function ------------------------ */

// Individual * FitnessFunction::bestIndividual = (0);

FitnessFunction::FitnessFunction(){}

int FitnessFunction::evaluate(Individual &ind){
    return -1;
}

void FitnessFunction::display(){
    cout << "Base fitness function" << endl;
}

/* ------------------------ OneMax Fitness Function ------------------------ */

int OneMax::evaluate(Individual &ind) {
    int result = 0;
    for(int i = 0; i < ind.genotype.size(); i++){
        result += ind.genotype[i];
    }
    if(result > bestIndividual->fitness){
        bestIndividual = &ind;
    }
    return result;
}   

void OneMax::display() {
    cout << "OneMax fitness function" << endl;
}