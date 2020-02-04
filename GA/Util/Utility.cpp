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
using namespace nlohmann;
using namespace arma;

vector<int> Utility::getOrderedArray(int n, Order order){
    switch (order) {
        case Order::RANDOM:
            return getRandomlyPermutedArrayV2(n);
        case Order::ASCENDING:
            return getAscendingArray(n);
        case Order::DESCENDING:
            return getDescendingArray(n);
    }
}

string Utility::orderToID(Order order){
    switch (order) {
        case Order::RANDOM:
            return "rand";
        case Order::ASCENDING:
            return "asc";
        case Order::DESCENDING:
            return "desc";
    }
}

string Utility::orderToString(Order order){
    switch (order) {
        case Order::RANDOM:
            return "Random";
        case Order::ASCENDING:
            return "Ascending";
        case Order::DESCENDING:
            return "Descending";
    }
}

vector<int> Utility::getRandomlyPermutedArray (int n){
    vector<int> arr;
    arr.reserve(n);
    for (int i = 0; i < n; i++) arr.push_back(i);
    shuffle(arr.begin(), arr.end(), default_random_engine());
    return arr;
}

vector<int> Utility::getRandomlyPermutedArrayV2 (int n){
    vector<int> arr = Utility::getAscendingArray(n);
    
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

vector<int> Utility::getAscendingArray(int n){
    vector<int> arr;
    arr.reserve(n);
    for(int i = 0; i < n; i++)
        arr.push_back(i);
    return arr;
}

vector<int> Utility::getDescendingArray(int n){
    vector<int> arr;
    arr.reserve(n);
    for(int i = n-1; i >= 0; i--)
        arr.push_back(i);
    return arr;
}

double Utility::getRand(){
    return dist(rng);
}

int Utility::getRand(int begin, int end){
    return floor(begin + (end - begin) * getRand());
}

int Utility::getConditionalBit(int counter0, int counter1, int max){
    int leftFor0 = (max / 2) - counter0;
    int leftFor1 = (max / 2) - counter1;
    int totalLeft = leftFor0 + leftFor1;
    
    float chanceOn0 = (float)leftFor0 / totalLeft;
    if (getRand() < chanceOn0){
        return 0;
    } else {
        return 1;
    }
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

string Utility::removeTrailingZeros(string target){
    int lastNonZero = target.size();
    for (int i = target.size() - 1; i >= 0; i--){
        if(target.at(i) != '0'){
            lastNonZero = i;
            break;
        }
    }
    return target.substr(0, lastNonZero + 1);
}

string Utility::padWithSpacesAfter(string target, int length){
    int n = target.size();
    for (int i = 0; i < length - n; i++)
        target = target + " ";
    return target;
}

void Utility::write(string content, string dir, string filename){
    ofstream file;
    file.open (dir + filename);
    file << content;
    file.close();
}

void Utility::writeRawData(string content, string dir, string suffix){
    if(suffix != "") suffix = "_" + suffix;
    write(content, dir, getDateString() + "_rawdata" + suffix + ".json");
}

void Utility::writeJSON(json content, string filename){
    ofstream file;
    file.open("/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/" + filename);
    file << content.dump();
    file.close();
}

json Utility::readJSON(string filename){
    ifstream file;
    file.open(filename);
    if(!file){
        cerr << "Unable to open file " + filename;
        exit(1);   // call system to stop
    }
    json result = json::parse(file);
    return result;
}

void Utility::read(string filename){
    ifstream file;
    file.open(filename);
    if(!file){
        cerr << "Unable to open file " + filename;
        exit(1);   // call system to stop
    }
    string s;
    while (file >> s) {
        cout << s;
    }
    cout << endl;
    file.close();
}

string Utility::genotypeToString(arma::uvec &genotype){
    string result = "";
    for(int i : genotype)
        result += to_string(i);
    return result;
}

uvec Utility::stringToGenotype (string &genotype){
    int n = genotype.size();
    uvec result(n);
    for (int i = 0; i < n; i++){
        result[i] = std::stoi(genotype.substr(i,1));
    }
    return result;
}

uvec Utility::vectorToUvec (vector<int> vec){
    uvec result(vec.size());
    for(int i = 0; i < vec.size(); i++){
        result[i] = vec[i];
    }
    return result;
}

vector<int> Utility::uvecToVector (uvec vec){
    vector<int> result;
    result.reserve(vec.size());
    for(int i = 0; i < vec.size(); i++){
        result.push_back(vec[i]);
    }
    return result;
}

string Utility::vecOfFloatsToString (vector<float> vec, string separator){
    string result = "";
    for (int i = 0; i < vec.size(); i++){
        result += to_string(vec[i]);
        if (i < vec.size()-1){
            result += separator;
        }
    }
    return result;
}

string Utility::wrapWithBrackets (string str){
    return ("[" + str + "]");
}
