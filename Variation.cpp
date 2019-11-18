#include "Variation.h"

using namespace std;

/* ------------------------ Base Variation ------------------------ */


Variation::Variation(){}

void Variation::variate(){
    cout << "Do a variation" << endl;
}

void Variation::display(){
    cout << "Base variation" << endl;
}


/* ------------------------ Univariate Crossover Variation ------------------------ */

UnivariateCrossover::UnivariateCrossover(){}

vector<Individual> UnivariateCrossover::variate(vector<Individual> &population){
    int n = population.size();
    vector<int> randomIndices;
    randomIndices.reserve(n);
    for (int i = 0; i < n; i++) randomIndices.push_back(i);
    shuffle(randomIndices.begin(), randomIndices.end(), default_random_engine(0));
    vector<Individual> newPopulation;
    newPopulation.reserve(n);

    for(int i = 0; i < (n/2); i++){
        int idx1 = randomIndices[i * 2];
        int idx2 = randomIndices[i * 2 + 1];

        vector<Individual> offspring = univariateCrossover(population[idx1], population[idx2]);

        newPopulation.push_back(offspring[0]);
        newPopulation.push_back(offspring[1]);
    }

    return newPopulation;
}

vector<Individual> UnivariateCrossover::univariateCrossover (Individual &ind1, Individual &ind2){
    vector<Individual> result;

    Individual newInd1 = ind1.copy();
    Individual newInd2 = ind2.copy();

    for(int i = 0; i < ind1.genotype.size(); i++){
        if(((float)rand() / RAND_MAX) < 0.5){
            newInd1.genotype[i] = ind2.genotype[i];
            newInd2.genotype[i] = ind1.genotype[i];
        }
    }

    result.push_back(newInd1);
    result.push_back(newInd2);
    return result;
}

void UnivariateCrossover::display() {
    cout << "Univariate Crossover" << endl;

}
