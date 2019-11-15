#include <random>
using namespace std;

class RNG {
    public:
        minstd_rand rng = default_random_engine(0);

        float next (){
            return ((float)rand() / RAND_MAX);
        }
};