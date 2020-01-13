//
//  ARK4.cpp
//  GA
//
//  Created by Tom den Ottelander on 13/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK4.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

ARK4::ARK4() : ARK(10, true, -1, ark4_optima[10], getProblemType(true)) {}

ARK4::ARK4(int problemSize, bool allowIdentityLayers, int maxEvaluations) : ARK(problemSize, allowIdentityLayers, maxEvaluations, ark4_optima[problemSize], getProblemType(allowIdentityLayers)) {}

void ARK4::display(){
    cout << "ARK-4 fitness function" << endl;
}

string ARK4::id(){
    if(allowIdentityLayers)
        return("ARK-4_inclID");
    else
        return("ARK-4_exclID");
}

FitnessFunction* ARK4::clone() const {
    return new ARK4(static_cast<const ARK4&>(*this));
}

// Returns the fitness of the architecture passed by its encoding by querying the benchmark
float ARK4::query(uvec encoding){
    //TODO: Implement this
    //Transform encoding into string
    string layers;
    for (int i : encoding){
        //2 is the identity layer
        if(i != 2){
            layers += to_string(i);
        }
    }
    
    //Prepend "model_"
    //Append ".json"
    //    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/nas_res/model_" + layers + ".json";
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/ark4/model_" + layers + ".json";
    
    //    cout << layers << endl;
    
    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    
    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(4);
    
    //return result
    return result;
}

int ARK4::getNumParams(vector<int>  encoding){
    string layers;
    for (int i : encoding){
        //2 is the identity layer
        if(i != 2){
            layers += to_string(i);
        }
    }
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/ark4/model_" + layers + ".json";
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    return rawdata["number_of_parameters"];
}

float ARK4::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
}

ProblemType* ARK4::getProblemType(bool allowIdentityLayers){
    // 0 = 3x3 convolution
    // 1 = 2x2 max pooling
    // 2 = identity
    vector<int> alphabet;
    if(allowIdentityLayers)
        alphabet = {0,1,2};
    else
        alphabet = {0,1};
    return new AlphabetProblemType(alphabet);
}

void ARK4::setLength (int length) {
    totalProblemLength = length;
}

uvec ARK4::transform(uvec &genotype){
    return removeIdentities(genotype, 2);
}
