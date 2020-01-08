//
//  ARK3.cpp
//  GA
//
//  Created by Tom den Ottelander on 07/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK3.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

ARK3::ARK3() : ARK(12, false, -1, ark3_optimum) {}

ARK3::ARK3(int maxEvaluations) : ARK(12, false, maxEvaluations, ark3_optimum) {
    setProblemType();
}

void ARK3::display(){
    cout << "ARK-3 fitness function" << endl;
}

string ARK3::id(){
    if(allowIdentityLayers)
        return("ARK-3_inclID");
    else
        return("ARK-3_exclID");
}

FitnessFunction* ARK3::clone() const {
    return new ARK3(static_cast<const ARK3&>(*this));
}

// Returns the fitness of the architecture passed by its encoding by querying the benchmark
float ARK3::query(uvec encoding){
    //Transform encoding into string
    string layers;
    for (int i : encoding){
        layers += to_string(i);
    }
    
    //Prepend "model_"
    //Append ".json"
    //    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/nas_res/model_" + layers + ".json";
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/ark3/model_" + layers + ".json";
    
    //    cout << layers << endl;
    
    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    
    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(4);
    
    //return result
    return result;
}

float ARK3::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
}

void ARK3::setProblemType(){
    // 0 = 3x3 convolution
    // 1 = 2x2 max pooling
    FitnessFunction::setProblemType(new AlphabetProblemType({0,1}));
}

void ARK3::setLength (int length) {
    totalProblemLength = length;
}

uvec ARK3::transform(uvec &genotype){
    return removeIdentities(genotype, 2);
}
