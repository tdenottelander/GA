//
//  ARK7.cpp
//  GA
//
//  Created by Tom den Ottelander on 29/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK7.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

extern std::string ARK_Analysis_suffix;

ARK7::ARK7(int problemSize, bool genotypeChecking) : ARK(problemSize, false, genotypeChecking, getProblemType(), -1, 2, "ark7")
{
    if(lookupTable.empty()){
        cout << "Reading in ARK-7" + ARK_Analysis_suffix + " results. This may take a while." << endl;
        string filename = folderPrefix + folder + "/ark7" + ARK_Analysis_suffix + ".json";
        ifstream ifs(filename);
        if(!ifs.good()){
            cout << "Error, cannot read results." << endl;
        } else {
            lookupTable = json::parse(ifs);
            cout << "Done loading ARK-7" + ARK_Analysis_suffix + " results" << endl;
        }
    }
}


float ARK7::getFitness (uvec encoding){
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
    
    float result = lookupTable[layers]["val_acc"];
    return result;
}

void ARK7::display(){
    cout << "ARK-7 fitness function" << endl;
}

string ARK7::id(){
    return "ARK-7";
}

FitnessFunction* ARK7::clone() const {
    return new ARK7(static_cast<const ARK7&>(*this));
}

ProblemType* ARK7::getProblemType (){
    //    vector<int> alphabet;
    //    alphabet = {0,1,2};
    return new AlphabetProblemType({0,1,2});
}
