//
//  Utility.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "Utility.hpp"

using namespace std;
using namespace chrono;

vector<int> Utility::getRandomlyPermutedArray (int n){
    vector<int> arr;
    arr.reserve(n);
    for (int i = 0; i < n; i++) arr.push_back(i);
    shuffle(arr.begin(), arr.end(), default_random_engine());
    return arr;
}

vector<int> Utility::getRandomlyPermutedArrayV2 (int n){
    vector<int> arr;
    arr.reserve(n);
    for (int i = 0; i < n; i++) arr.push_back(i);
    
    vector<int> result;
    result.reserve(n);
    for (int i = 0; i < n; i++){
        float rand = getRand();
        int idx = floor(rand * arr.size());
        result.push_back(arr[idx]);
        arr.erase(arr.begin()+idx);
    }
    
    return result;
}

double Utility::getRand(){
    return dist(rng);
}

long Utility::millis(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

string Utility::getDateString(){
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    string result = to_string(now->tm_year - 100);
    result += padFrontWith0(to_string(now->tm_mon + 1), 2);
    result += padFrontWith0(to_string(now->tm_mday), 2);
    result += "_";
    result += padFrontWith0(to_string(now->tm_hour), 2);
    result += padFrontWith0(to_string(now->tm_min), 2);
    result += padFrontWith0(to_string(now->tm_sec), 2);
    return result;
}

string Utility:: padFrontWith0(string target, int length){
    int curLength = target.size();
    for (int i = 0; i < (length - curLength); i++) target = "0" + target;
    return target;
}

void Utility::write(string content, string dir){
    ofstream file;
    file.open (dir + getDateString() + "_rawdata.json");
    file << content;
    file.close();
}

void Utility::read(string filename){
    ifstream file;
    file.open(filename);
    if(!file){
        cerr << "Unable to open file" + filename;
        exit(1);   // call system to stop
    }
    string s;
    while (file >> s) {
        cout << s;
    }
    cout << endl;
    file.close();
}
