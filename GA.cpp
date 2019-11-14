#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <algorithm>
#include <random>
#include <armadillo>

using namespace std;
using namespace arma;

float avgFitness = 0;
int n = 48;
int l = 25;
int gens = 100;
int runs = 100;
bool optFound = false;
auto rng = default_random_engine(0);

void printSolution(vector<int> vec){
    cout << "[";
    for (int i = 0; i < vec.size(); i++){
        cout << vec[i];
        if(i != (vec.size() - 1)){
            cout << " ";
        }
    }
    cout << "]";
}

int evaluate(vector<int> sol){
    int res = 0;
    for(int i = 0; i < sol.size(); i++){
        res += sol[i];
    }
    if(res == l){
        optFound = true;
    }
    return res;
}

float getAvgFitness(vector<vector<int>> population){
    int totalFitness = 0;
    int n = population.size();
    for(int i = 0; i < n; i++){
        totalFitness += evaluate(population[i]);
    }
    avgFitness = (float)totalFitness / n;
    return avgFitness;
}

vector<float> getProportions(vector<vector<int>> population){
    int n = population.size();
    vector<float> proportions (n);
    vector<int> fitness (n);
    int cumulativeFitness = 0;
    for(int i = 0; i < n; i++){
        int res = evaluate(population[i]);
        fitness[i] = res;
        cumulativeFitness += res;
    }

    avgFitness = (float)cumulativeFitness / n;

    for(int i = 0; i < n; i++){
        if(i == 0) {
            proportions[i] = (float)fitness[i] / cumulativeFitness;
        } else {
            proportions[i] = proportions[i-1] + (float)fitness[i] / cumulativeFitness;
        }
    }

    return proportions;
}

void printAvgFitness(){
    cout << "Average fitness of the population: " << avgFitness << endl;
}

void printPopulation(vector<vector<int>> pop){
    vector<float> proportions = getProportions(pop);
    for(int i = 0; i < pop.size(); i++){
        cout << i << ": ";
        printSolution(pop[i]);
        cout << "  fitness: " << evaluate(pop[i]); //<< " cumulative proportion: " << proportions[i];
        cout << endl;
    }
}

float getRand(){
    return (float)rand() / RAND_MAX;
}

uvec initSolution(int l){
    uvec sol (l);
    for(int i = 0; i < l; i++){
        sol[i] = getRand() < 0.5 ? 1 : 0;
    }
    return sol;
}

vector<vector<int>> initPopulation (int n, int l){
    vector<vector<int>> population;
    population.reserve(n);
    for (int i = 0; i < n; i++){
        // vector<int> individual = initSolution(l);
        // population.push_back(individual);
    }
    return population;
}

vector<vector<int>> univariateCrossover(vector<int> & sol1, vector<int> & sol2){

    vector<vector<int>> result;

    for(int i = 0; i < sol1.size(); i++){
        if(getRand() < 0.5){
            int temp = sol1[i];
            sol1[i] = sol2[i];
            sol2[i] = temp;
        }
    }

    result.push_back(sol1);
    result.push_back(sol2);

    return result;
}

int getRandomParentIndex (vector<vector<int>> population, vector<float> proportions){
    float r = getRand();
    for(int i = 0; i < population.size(); i++){
        if(r < proportions[i]){
            return i;
        }
    }
    return -1;
}

vector<vector<int>> selectionAndVariation (vector<vector<int>> population){
    vector<vector<int>> newPopulation;
    newPopulation.reserve(population.size());
    vector<float> proportions = getProportions(population);

    for(int i = 0; i < (population.size() / 2); i++){
        int parent1 = getRandomParentIndex(population, proportions);
        int parent2 = getRandomParentIndex(population, proportions);

        vector<vector<int>> offspring = univariateCrossover(population[parent1], population[parent2]);
        newPopulation.push_back(offspring[0]);
        newPopulation.push_back(offspring[1]);
    }

    return newPopulation;
}

vector<vector<int>> tournament2(vector<vector<int>> population){
    vector<int> randomIndices;
    randomIndices.reserve(2 * n);
    for (int i = 0; i < (2*n); i++) randomIndices.push_back(i);
    shuffle(randomIndices.begin(), randomIndices.end(), rng);
    vector<vector<int>> newPopulation;
    newPopulation.reserve(n);

    for(int i = 0; i < n; i++){
        int individual1idx = randomIndices[i];
        int individual2idx = randomIndices[i+1];

        if(evaluate(population[individual1idx]) > evaluate(population[individual2idx])){
            newPopulation.push_back(population[individual1idx]);
        } else {
            newPopulation.push_back(population[individual2idx]);
        }
    }

    return newPopulation;
}

vector<vector<int>> tournament4(vector<vector<int>> population){
    vector<vector<int>> newPopulation;
    newPopulation.reserve(n);

    //Do the tournament twice, because we have 2*n indivudals and tournament size 4.
    for(int i = 0; i < 2; i++){

        //Create a random permuation of all individual indices.
        vector<int> randomIndices;
        randomIndices.reserve(2 * n);
        for (int i = 0; i < (2*n); i++) randomIndices.push_back(i);
        shuffle(randomIndices.begin(), randomIndices.end(), rng);

        for(int i = 0; i < (n/2); i++){
            //Take 4 indices from the random index vector
            vector<int> indices(randomIndices.cbegin() + i * 4, randomIndices.cbegin() + i * 4 + 4);

            //Calculate the fitness for each of the individuals
            vector<int> fitnesses;
            fitnesses.reserve(4);
            for(int x = 0; x < 4; x++){
                fitnesses.push_back(evaluate(population[indices[x]]));
            }

            //Define which of the individuals should win the tournament
            int bestIndividual = 0;
            for(int k = 0; k < 4; k++){
                bool bestFitness = true;
                for(int l = 0; l < 4; l++){
                    if(fitnesses[k] < fitnesses[l]){
                        bestFitness = false;
                        break;
                    }
                }
                if(bestFitness){
                    bestIndividual = k;
                }
            }

            // cout << "Fitnesses: ";
            // for (int x: fitnesses) cout << x << ", ";
            // cout << " | Best: " << bestIndividual << endl;

            // Put this winning individual in the population of the next generation
            newPopulation.push_back(population[indices[bestIndividual]]);
        }
    }

    return newPopulation;
}

vector<vector<int>> variationAndSelection(vector<vector<int>> & population){
    //Create a vector of random indices of all individuals in the population
    vector<int> randomIndices;
    randomIndices.reserve(n);
    for (int i = 0; i < n; i++) randomIndices.push_back(i);
    shuffle(randomIndices.begin(), randomIndices.end(), rng);

    //Generate 2 offspring for every 2 parents in a random order
    for(int i = 0; i < (n / 2); i++){
        int parent1idx = randomIndices[i*2];
        int parent2idx = randomIndices[i*2+1];

        vector<vector<int>> offspring = univariateCrossover(population[parent1idx], population[parent2idx]);

        population.push_back(offspring[0]);
        population.push_back(offspring[1]);
    }
    //Now we have P+O in population

    //Do tournament selection on P+O
    vector<vector<int>> newPopulation = tournament4(population);

    return newPopulation;
}

void GA(){
    cout << "Populationsize = " << n << " | Problem length = " << l << " | Generations = " << gens << endl;
    int optimalFound = 0;
    for (int i = 0; i < runs; i++){
        optFound = false;
        vector<vector<int>> population = initPopulation(n, l);
        // printPopulation(population);

        for(int i = 0; i < gens; i++){
            // cout << "\nGeneration " << i + 1 << endl;
            population = variationAndSelection(population);
            // cout << "Gen " << i << ": ";
            // getAvgFitness(population);
            // printPopulation(population);
            // printAvgFitness();
            if(optFound){
                // cout << "Optimal found after " << i+1 << " generations" << endl;
                cout << "1" << flush;
                optimalFound += 1;
                break;
            }
        }
        if(!optFound) cout << "0" << flush;
    }

    cout << "\nOptimal found in " << optimalFound << " of 100 rounds" << endl;
}

int main()
{
    // GA();

    // mat A = ones<mat>(4,5);
    // mat B = randu<mat>(4,5);

    // cout << A*B.t() << endl;

    // uvec x(10);
    // x.fill(0);

    uvec x = initSolution(20);

    cout << x << endl;

    return 0;
}

