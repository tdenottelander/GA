//
//  ARK6.cpp
//  GA
//
//  Created by Tom den Ottelander on 23/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK6.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

extern string ARK_Analysis_suffix;
extern string benchmarksDir;

ARK6::ARK6(int problemSize, bool genotypeChecking) : ARK(problemSize, false, genotypeChecking, getProblemType(), -1, 2, "ark6")
{
    if(lookupTable.empty()){
        cout << "Reading in ARK-6" + ARK_Analysis_suffix + "results. This may take a while." << endl;
        string filename = benchmarksDir + folder + "/ark6" + ARK_Analysis_suffix + ".json";
        ifstream ifs(filename);
        if(!ifs.good()){
            cout << "Error, cannot read results." << endl;
        } else {
            lookupTable = json::parse(ifs);
            cout << "Done loading ARK-6" + ARK_Analysis_suffix + " results" << endl;
        }
    }
}


vector<float> ARK6::getFitness (uvec encoding){
    string layers;
    for (int i = 0; i < 14; i++){
        // Appends identity layers to the back of the architecture when we are dealing with problem lengths < 14.
        if(i >= totalProblemLength){
            layers += "I";
        } else {
            if(encoding[i] == 0){
                layers += "I";
            } else {
                layers += to_string(encoding[i]);
            }
        }
    }

    float result = lookupTable[layers]["val_acc"][0];
    return vector<float>{result};
}

void ARK6::display(){
    cout << "ARK-6 fitness function" << endl;
}

string ARK6::id(){
    return "ARK-6";
}

FitnessFunction* ARK6::clone() const {
    return new ARK6(static_cast<const ARK6&>(*this));
}

ProblemType* ARK6::getProblemType (){
//    vector<int> alphabet;
//    alphabet = {0,1,2};
    return new AlphabetProblemType({0,1,2});
}
