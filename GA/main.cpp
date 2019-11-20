//
//  main.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include <random>
#include <iostream>
#include <armadillo>
#include "Individual.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "FitnessFunction.hpp"
#include "GA.hpp"
#include "Utility.hpp"

using namespace std;
using namespace arma;
using namespace Utility;

int main(int argc, const char * argv[]) {
    
    int l = 5;
    int n = 10;
    
    ProportionateSelection selection;
    OnePointCrossover variation;
    OneMax fitnessFunc(l);
    
    GA ga (n, l, &fitnessFunc, &selection, &variation);
    cout << ga.toString() << endl;
    
    for(int i = 0; i < 50; i++){
        ga.round();
        cout << "\nRound " << i << endl;
        cout << ga.toString() << endl;
        cout << "Best fitness: " << ga.fitFunc->bestIndividual.fitness << endl;

        if(ga.fitFunc->optimumFound){
            cout << "\nOptimum found after " << i << " rounds" << endl;
            cout << "Individual: " << ga.fitFunc->bestIndividual.toString() << endl;
            break;
        }
    }
    
//    auto x = randu(1);
//    double x = arma::arma_rng::randu<double>();
//    mat A = randu<mat>(4,5);
//    vec  v = ones<vec>(10);
//    vector<int> x = Utility::getRandomlyPermutedArray(5);

//    cout << x << endl;
    
    return 0;
}
