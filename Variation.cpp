#pragma once

#include <iostream>
using namespace std;

class Variation {
    public:
        void variate(){
            cout << "Do a variation" << endl;
        }
};

class UnivariateCrossover : public Variation {
    public:
        vector<Individual> variate(vector<Individual> &population){
            int n = population.size();
            vector<int> randomIndices = getRandomlyPermutedArray(population.size(), rng);
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

        vector<Individual> univariateCrossover (Individual &ind1, Individual &ind2){
            vector<Individual> result;

            Individual newInd1 = ind1.copy();
            Individual newInd2 = ind2.copy();

            for(int i = 0; i < ind1.genotype.size(); i++){
                if(getRand() < 0.5){
                    newInd1.genotype[i] = ind2.genotype[i];
                    newInd2.genotype[i] = ind1.genotype[i];
                }
            }

            result.push_back(newInd1);
            result.push_back(newInd2);
            return result;
        }
};