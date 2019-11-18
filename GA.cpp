#include <vector>
#include <string>

using namespace std;

class GA {
    public:
        int populationSize;
        FitnessFunction * fitFunc;
        Selection * selection;
        Variation * variation;
        vector<Individual> population;
        int problemLength;

        GA(int popSize, int probLength, FitnessFunction *f, Selection *s, Variation *v){
            populationSize = popSize;
            fitFunc = f;
            selection = s;
            variation = v;
            v->display();
            s->display();
            problemLength = probLength;

            initialize();
        }

        void initialize(){
            population.reserve(populationSize);
            for(int i = 0; i < populationSize; i++){
                Individual ind (problemLength);
                ind.initialize();
                ind.fitness = fitFunc->evaluate(ind);
                population.push_back(ind);
            }
        }

        void round(){
            // population = selection.select(population, population.size());
            selection->display();
            variation->display();
            // variation.variate();
        }

        double getAvgFitness(){
            double result = 0;
            for(Individual ind: population){
                result += ind.fitness;
            }
            return result / population.size();
        }

        string toString() {
            string result;
            for(Individual ind: population){
                result += ind.toString();
                result += "\n";
            }
            return result;
        }
};