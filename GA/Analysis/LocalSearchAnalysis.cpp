//
//  LocalSearchAnalysis.cpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "LocalSearchAnalysis.hpp"

using namespace std;
using namespace nlohmann;

bool LocalSearchAnalysis::findBestLocalGene(Individual &ind, int index, FitnessFunction* fitfunc){
    int bestLayer = ind.genotype[index];
    float bestFitness = ind.fitness[0];
    int currentLayer = ind.genotype[index];
    for(int layertype : fitfunc->problemType->alphabet){
        if(layertype == currentLayer)
            continue;
        ind.genotype[index] = layertype;
        float fitness = fitfunc->evaluate(ind);
        if(fitness > bestFitness){
            bestLayer = layertype;
            bestFitness = fitness;
        }
    }
    ind.genotype[index] = bestLayer;
    ind.fitness[0] = bestFitness;
    return currentLayer != bestLayer;
}

int LocalSearchAnalysis::localSearch(Individual &ind, FitnessFunction* fitfunc, string localSearchType){
    int n = ind.genotype.size();
    int evaluations = 0;
    bool converged = false;
    while(!converged){
        converged = true;
        vector<int> order;
        if(localSearchType == "random"){
            order = Utility::getRandomlyPermutedArrayV2(n);
        } else if (localSearchType == "ascending"){
            order = Utility::getAscendingArray(n);
        } else if (localSearchType == "descending"){
            order = Utility::getDescendingArray(n);
        }
        for(int i = 0; i < n; i++){
            bool changed = findBestLocalGene(ind, order[i], fitfunc);
            evaluations += fitfunc->problemType->alphabet.size() - 1;
//            cout << ind.toString() << " (evals: " << evaluations << ")" << endl;
            if(changed)
                converged = false;
        }
    }
    return evaluations;
}

void LocalSearchAnalysis::localSearchTests(ARK* fitfunc, int runs, string localSearchType){
    
    vector<int> alphabet = fitfunc->problemType->alphabet;
    int probSize = fitfunc->totalProblemLength;
    vector<int> evaluationsForSuccess;
    vector<int> evaluationsForFail;
    vector<float> resultingFitness;
    int optimumfound = 0;
    
    SolutionCounter initialSolutionSpace(alphabet.size(), probSize);
    SolutionCounter finalSolutionSpace(alphabet.size(), probSize);
    
    for(int i = 0; i < runs; i++){
        Individual ind(probSize, fitfunc->numObjectives);
        ind.initialize(alphabet);
//        int j = 16383;
//        ind.genotype = HashingFunctions::decode(j, 7, 4);
        
        initialSolutionSpace.put(ind.genotype);
        
        int evaluations = 1 + localSearch(ind, fitfunc, localSearchType);
        
        resultingFitness.push_back(ind.fitness[0]);
        
        if(ind.fitness[0] >= fitfunc->optimum - 0.01){
            optimumfound++;
            evaluationsForSuccess.push_back(evaluations);
        } else
            evaluationsForFail.push_back(evaluations);
        
        if (i % (runs/100) == 0){
            if( i % (runs/10) == 0)
                cout << (i / (runs/10));
            else
                cout << ".";
        }
        finalSolutionSpace.put(ind.genotype);
    }
    cout << endl;
    
    cout << "Optimum found " << optimumfound << "/" << runs << " times" << endl;
    
    double meanEvaluationsForSuccess = accumulate(evaluationsForSuccess.begin(), evaluationsForSuccess.end(), 0.0) / evaluationsForSuccess.size();
    double meanEvaluationsForFail = accumulate(evaluationsForFail.begin(), evaluationsForFail.end(), 0.0) / evaluationsForFail.size();
    
    double meanFitness = accumulate(resultingFitness.begin(), resultingFitness.end(), 0.0) / resultingFitness.size();
    
    cout << "Mean evaluations for success: " << meanEvaluationsForSuccess << endl;
    cout << "Mean evaluations for fail: " << meanEvaluationsForFail << endl;
    cout << "Mean fitness after local search: " << meanFitness << endl;
    
    json main;
    main["finalSolutionSpace"] = finalSolutionSpace.toJson(false);
    main["initialSolutionSpace"] = initialSolutionSpace.toJson(false);
    
    main["totalRuns"] = runs;
    main["optimumHits"] = optimumfound;
    main["relativeOptimumHit"] = optimumfound * 1.0 / runs;
    main["meanEvaluationsSuccess"] = meanEvaluationsForSuccess;
    main["meanEvaluationsFail"] = meanEvaluationsForFail;
    main["meanFitness"] = meanFitness;
    main["localSearchType"] = localSearchType;
    main["alphabet"] = alphabet;
    main["problemSize"] = probSize;
    
    Utility::writeRawData(main.dump(), "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/data/", "fitnesslandscape");
}
