#include <vector>
using namespace std;

vector<int> getRandomlyPermutedArray (int n, RNG rng){
    vector<int> arr;
    arr.reserve(n);
    for (int i = 0; i < n; i++) arr.push_back(i);
    shuffle(arr.begin(), arr.end(), rng.rng);
    return arr;
}