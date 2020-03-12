//
//  ARK7.cpp
//  GA
//
//  Created by Tom den Ottelander on 29/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK7.hpp"

using namespace std;
using namespace nlohmann;

extern string ARK_Analysis_suffix;
extern string benchmarksDir;

ARK7::ARK7(int problemSize, bool genotypeChecking, bool MO) : ARK(problemSize, false, genotypeChecking, getProblemType(), -1, 2, "ark7"), MO(MO)
{
    if(lookupTable.empty()){
        string filename = benchmarksDir + folder + "/ark7";

        if (MO){
            filename += "_MO";
        }
        filename += ARK_Analysis_suffix + ".json";
        cout << "Reading in ARK-7" + ARK_Analysis_suffix + " results from " + filename + ". This may take a while.... ";
        ifstream ifs(filename);
        if(!ifs.good()){
            cout << "ERROR: cannot read results." << endl;
            exit(0);
        } else {
            lookupTable = json::parse(ifs);
            cout << "Done loading ARK-7" + ARK_Analysis_suffix + " results" << endl;
        }
    }
    
    if(MO){
        setOptimum(vector<float>{-1, -1});
        setNumObjectives(2);
        
        string filename = benchmarksDir + folder + "/pareto.json";
        cout << "Reading in ARK-7" + ARK_Analysis_suffix + " pareto results from " + filename + ".... ";
        ifstream ifs(filename);
        if(!ifs.good()){
            cout << "ERROR: cannot read pareto file." << endl;
        } else {
            json output = json::parse(ifs);
            json paretoInformation = output[to_string(problemSize)]["fitness"];
            for (int i = 0; i < paretoInformation.size(); i++){
                trueParetoFront.push_back(paretoInformation[i]);
            }
            cout << "Done loading ARK-7 Pareto results" << endl;
        }
        optimalParetoFrontSize = trueParetoFront.size();
    }
}


vector<float> ARK7::getFitness (vector<int> encoding){
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
    
    float acc = lookupTable[layers]["val_acc"];
    if (MO){
        float mmacs = lookupTable[layers]["MMACs"];
        return vector<float> {acc * normalization[0], 1.0f - (mmacs * normalization[1])};
    } else {
        return vector<float>{acc * normalization[0]};
    }
}

void ARK7::display(){
    cout << "ARK-7 fitness function" << endl;
}

string ARK7::id(){
    string res = "ARK-7-";
    res += (numObjectives == 1 ? "SO" : "MO");
    return res;
}

FitnessFunction* ARK7::clone() const {
    return new ARK7(static_cast<const ARK7&>(*this));
}

ProblemType* ARK7::getProblemType (){
    //    vector<int> alphabet;
    //    alphabet = {0,1,2};
    return new AlphabetProblemType({0,1,2});
}
