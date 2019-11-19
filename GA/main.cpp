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
    
    ProportionateSelection selection;
    OnePointCrossover variation;
    OneMax fitnessFunc;
    
    GA ga (10, 10, &fitnessFunc, &selection, &variation);
    cout << ga.toString() << endl;
    
    for(int i = 0; i < 5; i++){
        ga.round();
        cout << ga.toString() << endl;
    }
    
//    auto x = randu(1);
//    double x = arma::arma_rng::randu<double>();
//    mat A = randu<mat>(4,5);
//    vec  v = ones<vec>(10);
//    vector<int> x = Utility::getRandomlyPermutedArray(5);

//    cout << x << endl;
    
    return 0;
}
