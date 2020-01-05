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

ARK::ARK(int problemSize, int maxEvaluations, float optimum) : FitnessFunction(optimum, maxEvaluations) {
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
//    //TODO: Implement this
//    //Transform encoding into string
//    string layers;
//    for (int i : encoding){
//        //3 is the identity layer
//        if(i != 3){
//            layers += to_string(i);
//        }
//    }
//
//    //Prepend "model_"
//    //Append ".json"
////    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/nas_res/model_" + layers + ".json";
//    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/" + data_folder + "/model_" + layers + ".json";
//
//    //Load file
//    ifstream ifs(layers);
//    json rawdata = json::parse(ifs);
//
//    //Retrieve the correct value (validation accuracy at epoch 120)
//    float result = rawdata["val_acc_ensemble"].at(7);
//
//    //return result
//    return result;
    return -1.0;
}

// Returns the fitness of the architecture passed by its encoding (in vector<int>)
float ARK::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
//    return -1.0;
}

void ARK::display(){
    cout << "ARK-base fitness function" << endl;
}

string ARK::id(){
    return ("ARK-base");
}

//void ARK::setProblemType(){}
//
//void ARK::setProblemType(bool allowIdentityLayers){
//    // 0 = 3x3 convolution
//    // 1 = 3x3 convolution with stride 2
//    // 2 = 5x5 convolution
//    // 3 = identity
//    vector<int> alphabet;
//    if(allowIdentityLayers)
//        alphabet = {0,1,2,3};
//    else
//        alphabet = {0,1,2};
//    FitnessFunction::setProblemType(new AlphabetProblemType(alphabet));
//}

//FitnessFunction* ARK::clone() const {
//    return new ARK(static_cast<const ARK&>(*this));
//}

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
