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

NASBenchV2::NASBenchV2() : FitnessFunction(100.0) {
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

void NASBenchV2::setProblemType(){
    // 0 = 3x3 convolution
    // 1 = 3x3 convolution with stride 2
    // 2 = 5x5 convolution
    // 3 = identity
    vector<int> alphabet = {0,1,2,3};
    FitnessFunction::setProblemType(new AlphabetProblemType(alphabet));
}

FitnessFunction* NASBenchV2::clone() const {
    return new NASBenchV2(static_cast<const NASBenchV2&>(*this));
}

void NASBenchV2::setLength (int length){
    totalProblemLength = length;
}

void NASBenchV2::greedyRun (){
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
            
            float result = query(temp);
            cout << "  acc: " << result;
            if (result > bestAcc){
                architecture[i] = j;
                bestAcc = result;
            }
            cout << endl;
        }
    }
}

void NASBenchV2::greedyRunBackward(){
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
            
            float result = query(temp);
            cout << "  acc: " << result;
            if (result > bestAcc){
                architecture[i] = j;
                bestAcc = result;
            }
            cout << endl;
        }
    }
}

void NASBenchV2::greedyBothWays(){
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
            printArchitecture(architecture);
            
            float result = query(architecture);
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
            printArchitecture(architecture);
            
            float result = query(architecture);
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

void NASBenchV2::greedyInsideOut(){
    int len = 7;
    vector<int> architecture (2, 3);
    for (int i = 0; i < len; i++){
        
        float bestAcc = -1.0;
        int bestOp = -1;
        
        int insertionpoint = architecture.size() / 2;
        architecture.insert(architecture.begin() + insertionpoint, 3);
        
        for (int j = 0; j < 3; j++){
            architecture[insertionpoint] = j;
            printArchitecture(architecture);
            
            float result = query(architecture);
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

void NASBenchV2::findBest (int length){
    vector<int> architecture (length, -1);
    findBestRecursion(length, architecture, 0);
}

void NASBenchV2::findBestRecursion(int length, vector<int> &temp, int idx){
    if (idx == length){
        float result = query (temp);
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
