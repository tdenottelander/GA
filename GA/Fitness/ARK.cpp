//
//  ARK.cpp
//  GA
//
//  Created by Tom den Ottelander on 16/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//
//  Contains a class ARK that derives from FitnessFunction.
//  This class is meant to process the results of the NAS Benchmark
//  that Arkadiy and Marco created.
//

#include "ARK.hpp"

using namespace arma;
using namespace std;
using namespace nlohmann;

ARK::ARK(int problemSize, bool allowIdentityLayers, int maxEvaluations, float optimum, ProblemType* problemType, int identity, int jsonAccuracyIndex, string folder) : FitnessFunction(optimum, maxEvaluations, problemType), allowIdentityLayers(allowIdentityLayers), identityLayer(identity), jsonAccuracyIndex(jsonAccuracyIndex), folder(folder) {
    totalProblemLength = problemSize;
//    setProblemType(allowIdentityLayers);
}

// Returns the fitness of an individual
float ARK::evaluate(Individual &ind){
    float fitness = query(ind.genotype);
    ind.fitness = fitness;
    
    evaluationProcedure(ind);
    return fitness;
}

//// Returns the fitness of the architecture passed by its encoding by querying the benchmark
float ARK::query(uvec encoding){
    //Transform encoding into string
    string layers;
    for (int i : encoding){
        //ignore identity layer
        if(i != identityLayer){
            layers += to_string(i);
        }
    }

    //Prepend "model_"
    //Append ".json"
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/" + folder + "/model_" + layers + ".json";

    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);

    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(jsonAccuracyIndex);

    //return result
    return result;
}

// Returns the fitness of the architecture passed by its encoding (in vector<int>)
float ARK::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
}

int ARK::getNumParams(vector<int> encoding){
    string layers;
    for (int i : encoding){
        if(i != identityLayer){
            layers += to_string(i);
        }
    }
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/" + folder + "/model_" + layers + ".json";
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    return rawdata["number_of_parameters"];
}

void ARK::display(){
    cout << "ARK-base fitness function" << endl;
}

string ARK::id(){
    return ("ARK-base");
}

void ARK::setLength (int length){
    totalProblemLength = length;
}

void ARK::printArchitecture(vector<int> architecture){
    for (int i = 0; i < architecture.size(); i++){
        if(architecture[i] != 3){
            cout << architecture[i];
        } else {
            cout << "_";
        }
        if(i != architecture.size() - 1){
            cout << ",";
        }
    }
//    cout << endl;
}

uvec ARK::removeIdentities(uvec &genotype, int identityLayerIndex){
    uvec newGenotype (genotype.size());
    int j = 0;
    for (int i = 0; i < genotype.size(); i++){
        if (genotype[i] != identityLayerIndex){
            newGenotype[j] = genotype[i];
            j++;
        }
    }
    return newGenotype.head(j);
}


uvec ARK::transform(uvec &genotype){
    return removeIdentities(genotype, identityLayer);
}
