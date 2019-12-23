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

NASBenchV2::NASBenchV2(int problemSize, bool allowIdentityLayers, int maxEvaluations) : FitnessFunction(optima[problemSize], maxEvaluations) {
    setProblemType(allowIdentityLayers);
}

// Returns the fitness of an individual
float NASBenchV2::evaluate(Individual &ind){
    float fitness = query(ind.genotype);
    ind.fitness = fitness;
    
    evaluationProcedure(ind);
    return fitness;
}

// Returns the fitness of the architecture passed by its encoding by querying the benchmark
float NASBenchV2::query(uvec encoding){
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
    layers = "/Users/tomdenottelander/Stack/#CS_MASTER/Afstuderen/projects/GA/nas_res/model_" + layers + ".json";
    
    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    
    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(7);
    
    //return result
    return result; 
}

// Returns the fitness of the architecture passed by its encoding (in vector<int>)
float NASBenchV2::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
}

void NASBenchV2::display(){
    cout << "NASBenchV2 fitness function" << endl;
}

string NASBenchV2::id(){
    return ("NASBenchV2");
}

void NASBenchV2::setProblemType(){}

void NASBenchV2::setProblemType(bool allowIdentityLayers){
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

FitnessFunction* NASBenchV2::clone() const {
    return new NASBenchV2(static_cast<const NASBenchV2&>(*this));
}

void NASBenchV2::setLength (int length){
    totalProblemLength = length;
}

void GreedyAnalysis::greedyRun (){
    vector<int> architecture (7, -1);
    for (int i = 0; i < 7; i++){
        
        float bestAcc = -1.0;
        
        //Copying what we have so far
        vector<int> temp (i+1, -1);
        for(int k = 0; k < i; k++){
            temp[k] = architecture[k];
        }
        
        for (int j = 0; j < 3; j++){
            temp[i] = j;
            for (int q = 0; q < i+1; q++){
                cout << temp[q] << ",";
            }
            
            float result = NASBenchV2::query(temp);
            cout << "  acc: " << result;
            if (result > bestAcc){
                architecture[i] = j;
                bestAcc = result;
            }
            cout << endl;
        }
    }
}

void GreedyAnalysis::greedyRunBackward(){
    int len = 7;
    vector<int> architecture (len, -1);
    for (int i = len-1; i >= 0; i--){
        
        float bestAcc = -1.0;
        
        //Copying what we have so far
        vector<int> temp (len-i, -1);
        for(int k = 0; k < len-i; k++){
            temp[k] = architecture[k+i];
        }
        
        for (int j = 0; j < 3; j++){
            temp[0] = j;
            for (int q = 0; q < len-i; q++){
                cout << temp[q] << ",";
            }
            
            float result = NASBenchV2::query(temp);
            cout << "  acc: " << result;
            if (result > bestAcc){
                architecture[i] = j;
                bestAcc = result;
            }
            cout << endl;
        }
    }
}

void GreedyAnalysis::greedyBothWays(){
    int len = 7;
    vector<int> architecture (20, 3);
    
    int beforeIdx = 10;
    int afterIdx = 11;
    
    for (int i = 0; i < len; i++){
        
        float bestAcc = -1.0;
        bool after = true;
        int bestOp = -1;
        
        //First check if we add something afterwards
        for (int j = 0; j < 3; j++){
            architecture[afterIdx] = j;
            NASBenchV2::printArchitecture(architecture);
            
            float result = NASBenchV2::query(architecture);
            cout << "  acc: " << result;
            if (result > bestAcc){
                bestOp = j;
                bestAcc = result;
            }
            cout << endl;
        }
        architecture[afterIdx] = 3;
        
        //Now check before
        for (int j = 0; j < 3; j++){
            architecture[beforeIdx] = j;
            NASBenchV2::printArchitecture(architecture);
            
            float result = NASBenchV2::query(architecture);
            cout << "  acc: " << result;
            if (result > bestAcc){
                after = false;
                bestOp = j;
                bestAcc = result;
            }
            cout << endl;
        }
        architecture[beforeIdx] = 3;
        
        if(after){
            architecture[afterIdx] = bestOp;
            afterIdx++;
        } else {
            architecture[beforeIdx] = bestOp;
            beforeIdx--;
        }
        cout << endl;
    }
}

void GreedyAnalysis::greedyInsideOut(){
    int len = 7;
    vector<int> architecture (2, 3);
    for (int i = 0; i < len; i++){
        
        float bestAcc = -1.0;
        int bestOp = -1;
        
        int insertionpoint = architecture.size() / 2;
        architecture.insert(architecture.begin() + insertionpoint, 3);
        
        for (int j = 0; j < 3; j++){
            architecture[insertionpoint] = j;
            NASBenchV2::printArchitecture(architecture);
            
            float result = NASBenchV2::query(architecture);
            cout << "  acc: " << result;
            if (result > bestAcc){
                bestOp = j;
                bestAcc = result;
            }
            cout << endl;
        }
        architecture[insertionpoint] = bestOp;
        cout << endl;
    }
}

void NASBenchV2::printArchitecture(vector<int> architecture){
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

void GreedyAnalysis::findBest (int length){
    vector<int> architecture (length, -1);
    findBestRecursion(length, architecture, 0);
}

void GreedyAnalysis::findBestRecursion(int length, vector<int> &temp, int idx){
    if (idx == length){
        float result = NASBenchV2::query (temp);
        if (result > bestSoFar){
            bestSoFar = result;
            cout << "best so far: ";
            for (int q = 0; q < temp.size(); q++){
                cout << temp[q] << ",";
            }
            cout << " with accuracy: " << result << endl;
        }
    } else {
        for (int i = 0; i < 3; i++){
            temp [idx] = i;
            findBestRecursion(length, temp, idx+1);
        }
    }
}

bool LocalSearchAnalysis::findBestLocalGene(Individual &ind, int index, FitnessFunction* fitfunc){
    int bestLayer = -1;
    float bestFitness = -1.0;
    int currentLayer = ind.genotype[index];
    for(int layertype : fitfunc->problemType->alphabet){
        ind.genotype[index] = layertype;
        float fitness = fitfunc->evaluate(ind);
        if(fitness > bestFitness){
            bestLayer = layertype;
            bestFitness = fitness;
        }
    }
    ind.genotype[index] = bestLayer;
    fitfunc->evaluate(ind);
    return currentLayer != bestLayer;
}

int LocalSearchAnalysis::localSearch(Individual &ind, FitnessFunction* fitfunc){
    int n = ind.genotype.size();
    int evaluations = 0;
    bool converged = false;
    while(!converged){
        converged = true;
        //        vector<int> order = Utility::getRandomlyPermutedArrayV2(n);
        //        vector<int> order = Utility::getAscendingArray(n);
        vector<int> order = Utility::getDescendingArray(n);
        for(int i = 0; i < n; i++){
            bool changed = findBestLocalGene(ind, order[i], fitfunc);
            evaluations += 2;
            //            cout << ind.toString() << endl;
            if(changed)
                converged = false;
        }
    }
    //    cout << endl;
    return evaluations;
}

void LocalSearchAnalysis::localSearchTests(){
    
    int runs = 1000;
    
    int probSize = 7;
    NASBenchV2 fitfunc(probSize, false, -1);
    vector<int> alphabet = {0,1,2};
    vector<int> evaluationsForSuccess;
    vector<int> evaluationsForFail;
    int optimumfound = 0;
    
    for(int i = 0; i < runs; i++){
        Individual ind(probSize);
        ind.initialize(alphabet);
        
        int evaluations = 1 + localSearch(ind, &fitfunc);
        
        //        cout << ind.toString() << endl;
        if(ind.fitness > 89.13){
            optimumfound++;
            evaluationsForSuccess.push_back(evaluations);
        } else
            evaluationsForFail.push_back(evaluations);
        
        if (i % (runs/100) == 0){
            if( i % (runs/10) == 0)
                cout << (i / (runs/10));
            else
                cout << ".";
        }
    }
    cout << endl;
    
    cout << "Optimum found " << optimumfound << "/" << runs << " times" << endl;
    
    double meanEvaluationsForSuccess = accumulate(evaluationsForSuccess.begin(), evaluationsForSuccess.end(), 0.0) / evaluationsForSuccess.size();
    double meanEvaluationsForFail = accumulate(evaluationsForFail.begin(), evaluationsForFail.end(), 0.0) / evaluationsForFail.size();
    
    cout << "Mean evaluations for success: " << meanEvaluationsForSuccess << endl;
    cout << "Mean evaluations for fail: " << meanEvaluationsForFail << endl;
}
