#include <iostream>
using namespace std;

class Selection {
    public:

        Selection(){}

        vector<Individual> select(vector<Individual> &population, int outputSize) {
            cout << "Do a base selection" << endl;
            vector<Individual> v;
            return v;
        }

        virtual void display(){
            cout << "Base selection" << endl;
        }
};

class ProportionateSelection : public Selection {
    public:

        ProportionateSelection(){}

        vector<Individual> select(vector<Individual> &population, int outputSize) {
            vector<Individual> selection;
            selection.reserve(outputSize);
            vector<float> proportions = getProportions(population);
            for(int i = 0; i < outputSize; i++){
                float randomValue = ((float)rand() / RAND_MAX);
                for (int j = 0; j < proportions.size(); j++){
                    if(randomValue < proportions[j]){
                        selection.push_back(population[i]);
                        break;
                    }
                }
            }
            return selection;
        }

        void display() override {
            cout << "Proportionate selection" << endl;
        }

        vector<float> getProportions(vector<Individual> &population){
            int n = population.size();
            vector<float> proportions (n);
            int cumulativeFitness = 0;

            for(Individual ind: population){
                cumulativeFitness += ind.fitness;
            }

            for(int i = 0; i < n; i++){
                if(i == 0) {
                    proportions[i] = (float)population[i].fitness / cumulativeFitness;
                } else {
                    proportions[i] = proportions[i-1] + (float)population[i].fitness / cumulativeFitness;
                }
            }

            return proportions;
        }
};

class TournamentSelection : public Selection {
    public:
    //TODO FINISH THIS
        void select(vector<Individual> &population, int outputSize, int tournamentSize){
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

                if(population[idx1].fitness > population[idx2].fitness){
                    newPopulation.push_back(population[idx1]);
                } else {
                    newPopulation.push_back(population[idx2]);
                }
            }
        }

        void display() override {
            cout << "Tournament selection" << endl;
        }
};