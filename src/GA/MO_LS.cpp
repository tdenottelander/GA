//
//  MO_LS.cpp
//  GA
//
//  Created by Tom den Ottelander on 07/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "MO_LS.hpp"

using namespace std;

extern int populationInitializationMode; // 0 = True Random, 1 = ARK (first all identity individual), 2 = Solvable
extern nlohmann::json JSON_MO_info;
extern nlohmann::json JSON_LS_Results;
extern bool saveLSArchiveResults;

MO_LS::MO_LS (FitnessFunction * fitfunc, Utility::Order order, bool loop, NewScalarization newScalarization) : GA(fitfunc), LS_order(order), loop(loop), newScalarization(newScalarization) {
    isLocalSearchAlgorithm = true;
}

void MO_LS::round(){
    if(roundsCount == 0){
        if(populationInitializationMode == 1){
            pair<float, float> scalarization = {0.0f, 1.0f};
//            cout << "New individual found: " << population[0].toString() << "  for scalarization " << scalarization.first << "|" << scalarization.second << endl;
            updateLSArchive(scalarization, population[0].fitness);
        } else {
            if (newScalarization != NewScalarization::RANDOM)
                scalarizationTargets.push(0.0f); // In the direction of efficient network (MMACS)
        }
        
        if (newScalarization != NewScalarization::RANDOM)
            scalarizationTargets.push(1.0f); // In the direction of good predicting network (validation accuracy)
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
        pair<float,float> sc {scalarization, 1.0f - scalarization};
//        cout << "New individual found: " << ind.toString() << "  for scalarization " << sc.first << "|" << sc.second << endl;
        if (saveLSArchiveResults){
             updateLSArchive(sc, ind.fitness);
             JSON_LS_Results["LS_converged_solutions"] = LS_archive.size();
             JSON_LS_Results["LS_archive"] = LS_archive;
        }
    }

    roundsCount++;
}

void MO_LS::updateLSArchive(pair<float, float> scalarization, vector<float> objectiveValues){
    /* SETTING THE SCALARIZATION VALUE AS THE MEAN OF TWO SOLUTIONS WITH IDENTICAL OBJECTIVE VALUES
    for (int i = 0; i < old_archive.size(); i++){
        if(equalObjectiveValues(objectiveValues, old_archive[i].second)){
            float meanScalarization = (scalarization.first + old_archive[i].first.first) / 2;
            pair<float, float> newScalarization {meanScalarization, 1.0f - meanScalarization};
            old_archive.erase(old_archive.begin() + i);
            old_archive.push_back(pair<pair<float, float>, vector<float>> {newScalarization, objectiveValues});
            cout << "Found same fitness with other scalarization:         F: " << old_archive[i].second[0] << "|" << old_archive[i].second[1] << "   scalarization: " << old_archive[i].first.first << "|" << old_archive[i].first.second << ".... new scalarization: " << newScalarization.first << "|" << newScalarization.second << endl;
            return;
        }
    }
     */
    LS_archive.push_back(pair<pair<float, float>, vector<float>> {scalarization, objectiveValues});
}

bool MO_LS::equalObjectiveValues(std::vector<float> &o1, std::vector<float> &o2){
    if (o1.size() != o2.size()){
        cout << "Objective values do not have the same size" << endl;
        return false;
    }
    for (int i = 0; i < o1.size(); i++){
        if (o1[i] != o2[i]){
            return false;
        }
    }
    return true;
}

void MO_LS::performLocalSearch(Individual &ind, vector<float> scalarization){
    bool changed = true;
//    cout << "Starting Individual: " << ind.toString() << endl;
    do {
        changed = false;
        vector<int> randIndexArray = Utility::getOrderedArray(ind.genotype.size(), LS_order);
        for (int i : randIndexArray){
//            Individual originalIndividual = ind.copy();
            Individual copiedIndividual = ind.copy();
            
            // It does not matter in which order we loop over the available layers, because we search exhaustively and two different layers will have epsilon chance to result in the same objective values.
            for (int bit : fitFunc_ptr->problemType->alphabet){
                copiedIndividual.genotype[i] = bit;
                
                if(fitFunc_ptr->isDone()){
                    return;
                }
                
                fitFunc_ptr->evaluate(copiedIndividual);
                if (copiedIndividual.dominates(ind, scalarization)){
                    ind.genotype[i] = bit;
                    ind.fitness = copiedIndividual.fitness;
//                    cout << "Improvement: " << ind.toString() << endl;
                    changed = true;
                }
            }
        }
    } while (changed && loop);
}

float MO_LS::getNewScalarizationTarget(){
    if (newScalarization == NewScalarization::RANDOM){
        return Utility::getRand();
    } else {
        // Sort the result array based on scalarization value
        sort(LS_archive.begin(), LS_archive.end(), [](pair<pair<float, float>, vector<float>> lhs, pair<pair<float, float>, vector<float>> rhs){
            return lhs.first.first < rhs.first.first;
        });
        float largestDistance = -1;
        vector<int> indices;
        for (int i = 0; i < LS_archive.size() - 1; i++){
            float dist = 0.0f;
            if (newScalarization == NewScalarization::OBJECTIVESPACE){
                dist = Utility::EuclideanDistanceSquared(LS_archive[i].second, LS_archive[i+1].second); // OBJECTIVE SPACE DISTANCE
            } else if (newScalarization == NewScalarization::SCALARIZATIONSPACE){
                dist = abs(LS_archive[i].first.first - LS_archive[i+1].first.first); // SCALARIZATION SPACE DISTANCE
            }
            
            if (dist == largestDistance){
                indices.push_back(i);
            }
            
            if (dist > largestDistance){
                indices.clear();
                indices.push_back(i);
                largestDistance = dist;
            }
        }
        
        int largestDistanceIdx = Utility::getRand(indices);
        float result = (LS_archive[largestDistanceIdx].first.first + LS_archive[largestDistanceIdx+1].first.first) / 2;
        return result;
    }
}


GA* MO_LS::clone() const {
    return new MO_LS(static_cast<const MO_LS&>(*this));
}

string MO_LS::id(){
    string loopString = loop ? "loop" : "noloop";
    string dirString = "";
    switch (newScalarization) {
        case NewScalarization::RANDOM: dirString = "randdir"; break;
        case NewScalarization::SCALARIZATIONSPACE: dirString = "scalarizationdir"; break;
        case NewScalarization::OBJECTIVESPACE: dirString = "objectivedir"; break;
    }
    return ("MO-LS-" + loopString + "-" + dirString);
}




//LS_Archive::LS_Archive (bool combineEntries) : combineEntries(combineEntries) {}
//
//bool LS_Archive::contains(std::vector<float> objectiveValues){
//    for (int i = 0; i < archive.size(); i++){
//        vector<float> objectiveValuesInArchive = archive[i].second;
//        if (objectiveValues.size() != objectiveValuesInArchive.size()){
//            cout << "Number of objectives do not match" << endl;
//            return false;
//        }
//        bool equal = true;
//        for (int j = 0; j < objectiveValuesInArchive.size(); j++){
//            if (objectiveValues[j] != objectiveValuesInArchive[j]){
//                equal = false;
//                break;
//            }
//        }
//        if (equal) return true;
//    }
//    return false;
//}
//
//void LS_Archive::put(std::pair<std::pair<float, float>, std::vector<float> > pair){
//    if (!combineEntries){
//        archive.push_back(pair);
//    } else {
//        // TODO
//    }
//}
//
//void LS_Archive::put(std::pair<float, float> scalarization, std::vector<float> objectiveValues){
//    put(pair<pair<float, float>, vector<float>> {scalarization, objectiveValues});
//}
//
//pair<float, float> LS_Archive::getScalarization(std::vector<float> objectiveValues){
//    for (int i = 0; i < archive.size(); i++){
//        vector<float> objectiveValuesInArchive = archive[i].second;
//        if (objectiveValues.size() != objectiveValuesInArchive.size()){
//            cout << "Number of objectives do not match" << endl;
//            return {-1, -1};
//        }
//        bool equal = true;
//        for (int j = 0; j < objectiveValuesInArchive.size(); j++){
//            if (objectiveValues[j] != objectiveValuesInArchive[j]){
//                equal = false;
//                break;
//            }
//        }
//        if (equal) return archive[i].first;
//    }
//    return {-1, -1};
//}
//
//vector<float> LS_Archive::getObjectivevalues(std::pair<float, float> scalarization){
//    for (int i = 0; i < archive.size(); i++){
//        if (scalarization.first == archive[i].first.first){
//            return archive[i].second;
//        }
//    }
//    return {-1};
//}
//
//
