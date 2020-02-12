//
//  MO_LS.cpp
//  GA
//
//  Created by Tom den Ottelander on 07/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "MO_LS.hpp"

using namespace std;

MO_LS::MO_LS (FitnessFunction * fitfunc, Utility::Order order, int targetAmountOfSolutions) : GA(fitfunc), LS_order(order), targetAmountOfSolutions(targetAmountOfSolutions) {
    isLocalSearchAlgorithm = true;
}

void MO_LS::round(){
    if(roundsCount == 0){
        scalarizationTargets.push(0.0f);
        scalarizationTargets.push(1.0f);
    }

    for (Individual &ind : population){
        if (scalarizationTargets.empty()){
            scalarizationTargets.push(getNewScalarizationTarget());
        }
        float scalarization = scalarizationTargets.front();
        scalarizationTargets.pop();
        ind.initialize(fitFunc_ptr->problemType->alphabet);
        fitFunc_ptr->evaluate(ind);
//        cout << "Performing LS in the direction of [" << scalarization << ", " << 1.0f - scalarization << "]" << endl;
        performLocalSearch(ind, vector<float>{scalarization, 1.0f - scalarization});
        fitFunc_ptr->updateElitistArchive(ind);
        pair<float,float> sc {scalarization, 1.0f - scalarization};
        vector<float> fit (ind.fitness);
        LS_archive.push_back(pair<pair<float, float>, vector<float>> {sc, fit});
        
        if(targetAmountOfSolutions != -1 && LS_archive.size() >= targetAmountOfSolutions){
            converged = true;
        }
    }
    
    roundsCount++;
}

void MO_LS::performLocalSearch(Individual &ind, vector<float> scalarization){
    vector<int> randIndexArray = Utility::getOrderedArray(ind.genotype.size(), LS_order);
    bool changed = true;
//    cout << "Starting Individual: " << ind.toString() << endl;
    while (changed){
        changed = false;
        for (int i : randIndexArray){
//            Individual originalIndividual = ind.copy();
            Individual copiedIndividual = ind.copy();
            for (int bit : fitFunc_ptr->problemType->alphabet){
                copiedIndividual.genotype[i] = bit;
                fitFunc_ptr->evaluate(copiedIndividual);
                if (copiedIndividual.dominates(ind, scalarization)){
                    ind.genotype[i] = bit;
                    ind.fitness = copiedIndividual.fitness;
//                    cout << "Improvement: " << ind.toString() << endl;
                    changed = true;
                }
            }
        }
    }
}

float MO_LS::getNewScalarizationTarget(){
    // Sort the result array based on scalarization value
    sort(LS_archive.begin(), LS_archive.end(), [](pair<pair<float, float>, vector<float>> lhs, pair<pair<float, float>, vector<float>> rhs){
        return lhs.first.first < rhs.first.first;
    });
    float largestDistance = -1;
    int largestDistanceIdx = -1;
    for (int i = 0; i < LS_archive.size() - 1; i++){
        float dist = Utility::EuclideanDistanceSquared(LS_archive[i].second, LS_archive[i+1].second);
        if (dist > largestDistance){
            largestDistance = dist;
            largestDistanceIdx = i;
        }
    }
    float result = (LS_archive[largestDistanceIdx].first.first + LS_archive[largestDistanceIdx+1].first.first) / 2;
    return result;
}


GA* MO_LS::clone() const {
    return new MO_LS(static_cast<const MO_LS&>(*this));
}

string MO_LS::id(){
    return "MO-LS";
}
