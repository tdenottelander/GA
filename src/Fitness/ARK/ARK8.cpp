//
//  ARK8.cpp
//  GA
//
//  Created by Tom den Ottelander on 09/03/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "ARK8.hpp"

using namespace std;
using namespace nlohmann;

extern string ARK_Analysis_suffix;
extern string benchmarksDir;

ARK8::ARK8(int problemSize, bool genotypeChecking, bool MO) : ARK(problemSize, false, genotypeChecking, getProblemType(), -1, 2, "ark8"), MO(MO)
{
    if(lookupTable.empty()){
        string filename = benchmarksDir + folder + "/ark8";
        
        if (MO){
            filename += "_MO";
        }
        filename += ARK_Analysis_suffix + ".json";
        cout << "Reading in ARK-8" + ARK_Analysis_suffix + " results from " + filename + ". This may take a while.... ";
        ifstream ifs(filename);
        if(!ifs.good()){
            cout << "ERROR: cannot read results." << endl;
        } else {
            lookupTable = json::parse(ifs);
            cout << "Done loading ARK-8" + ARK_Analysis_suffix + " results" << endl;
        }
    }
    
    if(MO){
        setOptimum(vector<float>{-1, -1});
        setNumObjectives(2);
        
        string filename = benchmarksDir + folder + "/pareto.json";
        cout << "Reading in ARK-8" + ARK_Analysis_suffix + " pareto results from " + filename + ".... ";
        ifstream ifs(filename);
        if(!ifs.good()){
            cout << "ERROR: cannot read pareto file." << endl;
            exit(0);
        } else {
            json output = json::parse(ifs);
            json paretoInformation = output[to_string(problemSize)]["fitness"];
            for (int i = 0; i < paretoInformation.size(); i++){
                trueParetoFront.push_back(paretoInformation[i]);
            }
            cout << "Done loading ARK-8 Pareto results" << endl;
        }
        optimalParetoFrontSize = trueParetoFront.size();
    }
    networkLibrary.type = SolutionLibrary::Type::ARK_BENCHMARK;
    storeNetworkUniqueEvaluations = true;
}


vector<float> ARK8::evaluate(Individual &ind){
    vector<float> fitness;
    bool totalNetworkUniqueEvaluationsUpdate = false;
    
    if (networkLibrary.contains(ind.genotype)){
        fitness = networkLibrary.get(ind.genotype);
    } else {
        fitness = getFitness(ind.genotype);
        networkLibrary.put(ind.genotype, fitness);
        totalNetworkUniqueEvaluations++;
        totalNetworkUniqueEvaluationsUpdate = true;
    }
    
    ind.fitness = fitness;
    
    evaluationProcedure(ind);
    
    if(totalNetworkUniqueEvaluationsUpdate && log(totalNetworkUniqueEvaluations)){
        logNetworkUniqueEvaluations();
    }
    
    return fitness;
}

vector<float> ARK8::getFitness (vector<int> encoding){
    string layers;
    for (int i = 0; i < 14; i++){
        // Appends identity layers to the back of the architecture when we are dealing with problem lengths < 14.
        if(i >= totalProblemLength){
            layers += "I";
        } else {
            if(encoding[i] == 0){
                layers += "I";
            } else {
                layers += to_string(encoding[i]);
            }
        }
    }
    
    float acc = lookupTable[layers]["val_acc"];
    if (MO){
        float mmacs = lookupTable[layers]["MMACs"];
        return vector<float> {acc * normalization[0], 1.0f - (mmacs * normalization[1])};
    } else {
        return vector<float>{acc * normalization[0]};
    }
}

void ARK8::display(){
    cout << "ARK-8 fitness function" << endl;
}

string ARK8::id(){
    string res = "ARK-8-";
    res += (numObjectives == 1 ? "SO" : "MO");
    return res;
}

FitnessFunction* ARK8::clone() const {
    return new ARK8(static_cast<const ARK8&>(*this));
}

ProblemType* ARK8::getProblemType (){
    //    vector<int> alphabet;
    //    alphabet = {0,1,2};
    return new AlphabetProblemType({0,1,2});
}
