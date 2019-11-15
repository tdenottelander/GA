#include <armadillo>
#include <string>

using namespace arma;
using namespace std;

namespace N {
    class Individual {
        public:
            uvec genotype;
            int fitness;
            Individual();
            void initialize();
            Individual copy();
            string toString();
    };
};