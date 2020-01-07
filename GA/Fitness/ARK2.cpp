//
//  ARK2.cpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK2.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

ARK2::ARK2() : ARK(13, true, -1, ark2_optima[13]) {}

ARK2::ARK2(int problemSize, bool allowIdentityLayers, int maxEvaluations) : ARK(problemSize, allowIdentityLayers, maxEvaluations, ark2_optima[problemSize]) {
    setProblemType(allowIdentityLayers);
}

void ARK2::display(){
    cout << "ARK-2 fitness function" << endl;
}

string ARK2::id(){
    if(allowIdentityLayers)
        return("ARK-2_inclID");
    else
        return("ARK-2_exclID");
}

FitnessFunction* ARK2::clone() const {
    return new ARK2(static_cast<const ARK2&>(*this));
}

// Returns the fitness of the architecture passed by its encoding by querying the benchmark
float ARK2::query(uvec encoding){
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
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/ark2/model_" + layers + ".json";
    
//    cout << layers << endl;
    
    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    
    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(4);
    
    //return result
    return result;
}

float ARK2::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
}

void ARK2::setProblemType(){}

void ARK2::setProblemType(bool allowIdentityLayers){
    // 0 = 3x3 convolution
    // 1 = 2x2 max pooling
    // 2 = identity
    vector<int> alphabet;
    if(allowIdentityLayers)
        alphabet = {0,1,2};
    else
        alphabet = {0,1};
    FitnessFunction::setProblemType(new AlphabetProblemType(alphabet));
}

void ARK2::setLength (int length) {
    totalProblemLength = length;
}

uvec ARK2::transform(uvec &genotype){
    return removeIdentities(genotype, 2);
}
