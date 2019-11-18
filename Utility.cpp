#include "Utility.h"

using namespace std;

vector<int> Utility::getRandomlyPermutedArray (int n){
    vector<int> arr;
    arr.reserve(n);
    for (int i = 0; i < n; i++) arr.push_back(i);
    shuffle(arr.begin(), arr.end(), default_random_engine(0));
    return arr;
}