class FitnessFunction {
    public:
        Individual bestIndividual = (0);

        FitnessFunction(){}

        virtual int evaluate(Individual &ind){
            return -1;
        }

        virtual void display(){
            cout << "Base fitness function" << endl;
        }
};

class OneMax : public FitnessFunction {
    public:
    int evaluate(Individual &ind) override {
        int result = 0;
        for(int i = 0; i < ind.genotype.size(); i++){
            result += ind.genotype[i];
        }
        if(result > bestIndividual.fitness){
            bestIndividual = ind;
        }
        return result;
    }   

    void display() override {
        cout << "OneMax fitness function" << endl;
    }
};