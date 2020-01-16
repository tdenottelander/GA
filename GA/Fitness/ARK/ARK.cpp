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

ARK::ARK(int problemSize, bool allowIdentityLayers, int maxEvaluations, ProblemType* problemType, int identity, int jsonAccuracyIndex, string folder) :
    FitnessFunction(getOptimum(folder, problemSize, allowIdentityLayers), maxEvaluations, problemType),
    allowIdentityLayers(allowIdentityLayers),
    identityLayer(identity),
    jsonAccuracyIndex(jsonAccuracyIndex),
    folder(folder) {
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
    layers = folderPrefix + folder + "/model_" + layers + ".json";

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
    layers = folderPrefix + folder + "/model_" + layers + ".json";
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


uvec ARK::transform(uvec &genotype){
    return Individual::removeIdentities(genotype, identityLayer);
}

pair<float, vector<int>> ARK::findBest (){
    vector<int> architecture (totalProblemLength, -1);
    pair<float, vector<int>> opt = findBestRecursion(totalProblemLength, problemType->alphabet.size(), architecture, 0, -1.0);
    cout << "Genotype with optimal fitness: ";
    cout << Individual::toString(opt.second);
    cout << "   accuracy: " << opt.first;
    cout << "   #params: " << getNumParams(opt.second) << endl;
    return opt;
}

pair<float, vector<int>> ARK::findBestRecursion(int length, int alphabetSize, vector<int> &temp, int idx, float bestSoFar){
    vector<int> bestGenotype(temp);
    if (idx == length){
        float result = query (temp);
        if (result > bestSoFar){
            bestSoFar = result;
            bestGenotype = temp;
        }
    } else {
        for (int i = 0; i < alphabetSize; i++){
            temp [idx] = i;
            pair<float, vector<int>> result = findBestRecursion(length, alphabetSize, temp, idx+1, bestSoFar);
            if (result.first > bestSoFar){
                bestSoFar = result.first;
                bestGenotype = result.second;
            }
        }
    }
    return pair<float, vector<int>>(bestSoFar, bestGenotype);
}

pair<int, int> ARK::findAmountOfArchitecturesWithFitnessAboveThreshold(float threshold){
    int sum = 0;
    int total = pow(problemType->alphabet.size(), totalProblemLength);
    for (int i = 0; i < total; i++){
        uvec genotype = HashingFunctions::decode(i, totalProblemLength, problemType->alphabet.size());
        float result = query(genotype);
        if(result >= threshold){
            cout << Individual::toString(genotype) << " f=" << result << endl;
            sum++;
        }
    }
    cout << sum << "/" << total << " genotypes have fitness >= " << threshold << endl;
    return pair<int, int>(sum, total);
}

void ARK::doAnalysis(int minLayerSize, int maxLayerSize){
    json results;
    json optima;
    for (int i = minLayerSize; i <= maxLayerSize; i++){
        totalProblemLength = i;
        pair<float, vector<int>> res = findBest();
        pair<float, string> opt(res.first, Individual::toString(res.second));
        optima[to_string(i)] = opt;
    }
    results["optima"] = optima;
    Utility::write(results.dump(), folderPrefix + folder + "/", "analysis.json");
}

float ARK::getOptimum(string folder, int layers, bool allowIdentityLayers){
    string filename = folderPrefix + folder + "/analysis.json";
    ifstream ifs(filename);
    if(!ifs.good()){
        cout << "Cannot load optima. Consider first running the function \"doAnalysis\" first. Setting optimum to 100.0 for now.";
        return 100.0;
    }
    cout << "Loading optima from " << filename << endl;
    json analysis = json::parse(ifs);
    float result = analysis["optima"][to_string(layers)][0];
    return result;
    return 0.0;
}
