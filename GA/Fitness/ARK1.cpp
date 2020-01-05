//
//  ARK1.cpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK1.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

ARK1::ARK1() : ARK(7, -1, ark1_optima[7]) {}

ARK1::ARK1(int problemSize, bool allowIdentityLayers, int maxEvaluations) : ARK(problemSize, maxEvaluations, ark1_optima[problemSize]) {
    setProblemType(allowIdentityLayers);
}

void ARK1::display(){
    std::cout << "ARK-1 fitness function" << std::endl;
}

std::string ARK1::id(){
    return("ARK-1");
}

FitnessFunction* ARK1::clone() const {
    return new ARK1(static_cast<const ARK1&>(*this));
}

// Returns the fitness of the architecture passed by its encoding by querying the benchmark
float ARK1::query(uvec encoding){
    //TODO: Implement this
    //Transform encoding into string
    string layers;
    for (int i : encoding){
        //3 is the identity layer
        if(i != 3){
            layers += to_string(i);
        }
    }
    
    //Prepend "model_"
    //Append ".json"
    //    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/nas_res/model_" + layers + ".json";
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/ark1/model_" + layers + ".json";
    
    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    
    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(7);
    
    //return result
    return result;
}

float ARK1::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
}

void ARK1::setProblemType(){}

void ARK1::setProblemType(bool allowIdentityLayers){
    // 0 = 3x3 convolution
    // 1 = 3x3 convolution with stride 2
    // 2 = 5x5 convolution
    // 3 = identity
    vector<int> alphabet;
    if(allowIdentityLayers)
        alphabet = {0,1,2,3};
    else
        alphabet = {0,1,2};
    FitnessFunction::setProblemType(new AlphabetProblemType(alphabet));
}

void ARK1::setLength (int length) {
    totalProblemLength = length;
}

