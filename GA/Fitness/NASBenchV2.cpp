//
//  NASBenchV2.cpp
//  GA
//
//  Created by Tom den Ottelander on 16/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//
//  Contains a class NASBenchV2 that derives from FitnessFunction.
//  This class is meant to process the results of the NAS Benchmark
//  that Arkadiy and Marco created.
//

#include "NASBenchV2.hpp"

using namespace arma;
using namespace std;
using namespace nlohmann;

NASBenchV2::NASBenchV2(){
    setProblemType();
}

float NASBenchV2::evaluate(Individual &ind){
    float fitness = query(ind.genotype);
    ind.fitness = fitness;
    
    evaluation(ind);
    return fitness;
}

float NASBenchV2::query(uvec encoding){
    //TODO: Implement this
    //Transform encoding into string
    string layers;
    for (int i : encoding){
        layers += to_string(i);
    }
    
    //Prepend "model_"
    //Append ".json"
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/nas_res/model_" + layers + ".json";
    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    
    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(7);
    
    //return result
    return result; 
}

void NASBenchV2::display(){
    cout << "NASBenchV2 fitness function" << endl;
}

string NASBenchV2::id(){
    return ("NASBenchV2");
}

void NASBenchV2::setProblemType(){
    vector<int> alphabet = {0,1,2};
    FitnessFunction::setProblemType(new AlphabetProblemType(alphabet));
}

FitnessFunction* NASBenchV2::clone() const {
    return new NASBenchV2(static_cast<const NASBenchV2&>(*this));
}

void NASBenchV2::setLength (int length){
    totalProblemLength = length;
}
