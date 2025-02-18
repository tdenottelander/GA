//
//  GreedyAnalysis.cpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "GreedyAnalysis.hpp"

using namespace std;

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
            
            vector<float> result = ARK1().query(temp);
            cout << "  acc: " << result[0];
            if (result[0] > bestAcc){
                architecture[i] = j;
                bestAcc = result[0];
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
            
            vector<float> result = ARK1().query(temp);
            cout << "  acc: " << result[0];
            if (result[0] > bestAcc){
                architecture[i] = j;
                bestAcc = result[0];
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
            ARK::printArchitecture(architecture);
            
            vector<float> result = ARK1().query(architecture);
            cout << "  acc: " << result[0];
            if (result[0] > bestAcc){
                bestOp = j;
                bestAcc = result[0];
            }
            cout << endl;
        }
        architecture[afterIdx] = 3;
        
        //Now check before
        for (int j = 0; j < 3; j++){
            architecture[beforeIdx] = j;
            ARK::printArchitecture(architecture);
            
            vector<float> result = ARK1().query(architecture);
            cout << "  acc: " << result[0];
            if (result[0] > bestAcc){
                after = false;
                bestOp = j;
                bestAcc = result[0];
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
            ARK::printArchitecture(architecture);
            
            vector<float> result = ARK1().query(architecture);
            cout << "  acc: " << result[0];
            if (result[0] > bestAcc){
                bestOp = j;
                bestAcc = result[0];
            }
            cout << endl;
        }
        architecture[insertionpoint] = bestOp;
        cout << endl;
    }
}
