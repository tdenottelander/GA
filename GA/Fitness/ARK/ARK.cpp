//
//  ARK.cpp
//  GA
//
//  Created by Tom den Ottelander on 16/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//
//  Contains a class ARK that derives from FitnessFunction.
//  This class is meant to process the results of the NAS Benchmark
//  that Arkadiy and Marco created.
//

#include "ARK.hpp"

using namespace arma;
using namespace std;
using namespace nlohmann;

extern std::string ARK_Analysis_suffix;

ARK::ARK(int problemSize, bool allowIdentityLayers, bool genotypeChecking, ProblemType* problemType, int identity, int jsonAccuracyIndex, string folder) :
    FitnessFunction(vector<float>(1, getOptimum(folder, problemSize, allowIdentityLayers)), problemType),
    allowIdentityLayers(allowIdentityLayers),
    identityLayer(identity),
    jsonAccuracyIndex(jsonAccuracyIndex),
    folder(folder) {
        
        totalProblemLength = problemSize;
        if(genotypeChecking){
            setGenotypeChecking();
        }
        
//    setProblemType(allowIdentityLayers);
}

// Returns the fitness of an individual
vector<float> ARK::evaluate(Individual &ind){
    vector<float> fitness = query(ind.genotype);
    if(noisy) {
        std::normal_distribution<> dist (0, noisePercentage * fitness[0]);
        float noise = dist(rng);
//        cout << "Fitness: " << fitness << " noise: " << noise << endl;
        fitness[0] += noise;
    }
    ind.fitness = fitness;
    
    evaluationProcedure(ind);
    return fitness;
}

// Returns the fitness of the architecture passed by its encoding by querying the benchmark
vector<float> ARK::query(uvec encoding){
    vector<float> result = getFitness(encoding);
    return result;
}

// Returns the fitness of the architecture passed by its encoding (in vector<int>)
vector<float> ARK::query(vector<int> encoding){
    uvec uvecEncoding(encoding.size());
    for (int i = 0; i < encoding.size(); i++){
        uvecEncoding[i] = encoding[i];
    }
    return query(uvecEncoding);
}

vector<float> ARK::getFitness(uvec encoding){
    //Transform encoding into string
    string layers;
    for (int i : encoding){
        //ignore identity layer
        if(i != identityLayer){
            layers += to_string(i);
        }
    }
    
    //Prepend "model_"
    //Append ".json"
    layers = folderPrefix + folder + "/model_" + layers + ".json";
    
    //Load file
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    
    //Retrieve the correct value (validation accuracy at epoch 120)
    float result = rawdata["val_acc_ensemble"].at(jsonAccuracyIndex);
    return vector<float>{result};
}

int ARK::getNumParams(vector<int> encoding){
    string layers;
    for (int i : encoding){
        if(i != identityLayer){
            layers += to_string(i);
        }
    }
    layers = folderPrefix + folder + "/model_" + layers + ".json";
    ifstream ifs(layers);
    json rawdata = json::parse(ifs);
    return rawdata["number_of_parameters"];
}

void ARK::display(){
    cout << "ARK-base fitness function" << endl;
}

string ARK::id(){
    return ("ARK-base");
}

void ARK::setLength (int length){
    totalProblemLength = length;
}

void ARK::printArchitecture(vector<int> architecture){
    for (int i = 0; i < architecture.size(); i++){
        if(architecture[i] != 3){
            cout << architecture[i];
        } else {
            cout << "_";
        }
        if(i != architecture.size() - 1){
            cout << ",";
        }
    }
//    cout << endl;
}


uvec ARK::transform(uvec &genotype){
    return Individual::removeIdentities(genotype, identityLayer);
}

ARK::solution ARK::findBest (){
    vector<int> architecture (totalProblemLength, -1);
    solution statistics;
    statistics.fitness = -1.0;
    statistics.genotypes = {};
    statistics.optCount = 0;
    statistics.totalCount = 0;
    elitistArchive.clear();
    
    findBestRecursion(totalProblemLength, problemType->alphabet.size(), architecture, 0, statistics);
    
    cout << "Genotypes with optimal fitness: " << endl;
    for (vector<int> gen : statistics.genotypes){
        cout << Individual::toString(gen);
    }
    cout << "   accuracy: " << statistics.fitness;
//    cout << "   #params: " << getNumParams(opt.second) << endl;
    cout << endl;
    return statistics;
}

void ARK::findBestRecursion(int length, int alphabetSize, vector<int> &temp, int idx, solution &statistics){
    if (idx == length){
        vector<float> result = query (temp);
        Individual ind;
        ind.fitness = result;
        uvec gen = Utility::vectorToUvec(temp);
        ind.genotype = gen;
        updateElitistArchive(ind);
        if (abs(result[0] - statistics.fitness) < 0.001){
            statistics.genotypes.push_back(temp);
            statistics.optCount = statistics.optCount + 1;
        } else if (result[0] > statistics.fitness){
            statistics.fitness = result[0];
            statistics.genotypes = {temp};
            statistics.optCount = 1;
        }
        statistics.totalCount = statistics.totalCount + 1;
    } else {
        for (int i = 0; i < alphabetSize; i++){
            temp [idx] = i;
            findBestRecursion(length, alphabetSize, temp, idx+1, statistics);
        }
    }
}

pair<int, int> ARK::findAmountOfArchitecturesWithFitnessAboveThreshold(float threshold){
    int sum = 0;
    int total = pow(problemType->alphabet.size(), totalProblemLength);
    for (int i = 0; i < total; i++){
        uvec genotype = HashingFunctions::decode(i, totalProblemLength, problemType->alphabet.size());
        vector<float> result = query(genotype);
        if(result[0] >= threshold){
            cout << Individual::toString(genotype) << " f=" << result[0] << endl;
            sum++;
        }
    }
    cout << sum << "/" << total << " genotypes have fitness >= " << threshold << endl;
    return pair<int, int>(sum, total);
}

void ARK::doAnalysis(int minLayerSize, int maxLayerSize){
    json results;
    json optima;
    json paretoFronts;
    
    for (int i = minLayerSize; i <= maxLayerSize; i++){
        totalProblemLength = i;
        solution statistics = findBest();
        vector<string> genotypes;
        for(vector<int> genotype : statistics.genotypes){
            string genotypeString;
            for(int gene : genotype){
                genotypeString += to_string(gene);
            }
            genotypes.push_back(genotypeString);
        }
        pair<float, vector<string>> opt(statistics.fitness, genotypes);
        optima[to_string(i)]["optimum"] = statistics.fitness;
        optima[to_string(i)]["genotypes"] = genotypes;
        optima[to_string(i)]["numGlobalOptima"] = statistics.optCount;
        optima[to_string(i)]["possibleGenotypes"] = statistics.totalCount;
        json paretoFront;
        json paretoFrontFitness;
        json paretoFrontGenotypes;
        sort(elitistArchive.begin(), elitistArchive.end(), [](const Individual lhs, const Individual rhs){
            if (lhs.fitness[0] > rhs.fitness[0]){
                return true;
            } else if (lhs.fitness[0] < rhs.fitness[0]){
                return false;
            } else {
                return lhs.fitness[1] > rhs.fitness[1];
            }
        });
        for (int j = 0; j < elitistArchive.size(); j++){
            paretoFrontFitness[j] = elitistArchive[j].fitness;
            paretoFrontGenotypes[j] = Utility::genotypeToString(elitistArchive[j].genotype);
        }
        paretoFront["fitness"] = paretoFrontFitness;
        paretoFront["genotypes"] = paretoFrontGenotypes;
        paretoFronts[to_string(i)] = paretoFront;
    }
    results["optima"] = optima;
    Utility::write(results.dump(), folderPrefix + folder + "/", "analysis" + ARK_Analysis_suffix + "_" + Utility::getDateString() + ".json");
    Utility::write(paretoFronts.dump(), folderPrefix + folder + "/", "paretofront_" + Utility::getDateString() + ".json");
}

float ARK::getOptimum(string folder, int layers, bool allowIdentityLayers){
    string filename = folderPrefix + folder + "/analysis" + ARK_Analysis_suffix + ".json";
    ifstream ifs(filename);
    if(!ifs.good()){
        cout << "Cannot load optima. Consider first running the function \"doAnalysis\" first. Setting optimum to 100.0 for now." << endl;
        return 100.0;
    }
    cout << "Loading optima from " << filename << endl;
    json analysis = json::parse(ifs);
//    auto optGenotypes = analysis["optima"][to_string(layers)]["genotypes"];
//    vector<uvec> genotypes;
//    for (string gen : optGenotypes){
//        cout << gen << endl;
//        genotypes.push_back(Utility::stringToGenotype(gen));
//    }
//    int selectedGenotypeIdx = findMostDifferentGenotype(genotypes);
//    optimalGenotype = uvec(genotypes[selectedGenotypeIdx]);
//    cout << "optimal Genotype: " << Utility::genotypeToString(optimalGenotype);
    float result = analysis["optima"][to_string(layers)]["optimum"];
    return result;
}

uvec ARK::getOptimalGenotype(){
    string filename = folderPrefix + folder + "/analysis" + ARK_Analysis_suffix + ".json";
    ifstream ifs(filename);
    if (!ifs.good()){
        cout << "Cannot load optima. Consider first running the function \"doAnalysis\" first. Setting optimal genotype to {0}." << endl;
        return {0};
    } else {
        json analysis = json::parse(ifs);
        auto optGenotypes = analysis["optima"][to_string(totalProblemLength)]["genotypes"];
        vector<uvec> genotypes;
        for (string gen : optGenotypes){
            cout << gen << endl;
            genotypes.push_back(Utility::stringToGenotype(gen));
        }
        int selectedGenotypeIdx = findMostDifferentGenotype(genotypes);
        uvec optGen = uvec(genotypes[selectedGenotypeIdx]);
        cout << "optimal Genotype: " << Utility::genotypeToString(optGen) << endl;
        return optGen;
    }
}

void ARK::setGenotypeChecking(){
    uvec optGen = getOptimalGenotype();
    FitnessFunction::setGenotypeChecking(optGen);
}

int ARK::findMostDifferentGenotype(vector<arma::uvec> &genotypes){
    bool print = true;
    vector<vector<int>> distances;
    int n = genotypes.size();
    distances.reserve(n);
    for (int i = 0; i < n; i++){
        vector<int> distancesInner;
        distancesInner.reserve(n);
        distances.push_back(distancesInner);
    }
    
    int highestDist = -1;
    int mostDifferentGenotypeIdx = -1;
    for (int i = 0; i < genotypes.size(); i++){
        if (print) cout << "[ ";
        int total = 0;
        for (int j = 0; j < genotypes.size(); j++){
            int dist = Individual::hammingDistance(genotypes[i], genotypes[j]);
            distances[i].push_back(dist);
            if (print) cout << dist << " ";
            total += dist;
        }
        if (total > highestDist){
            highestDist = total;
            mostDifferentGenotypeIdx = i;
        }
        if (print) cout << "] (" << total << ")\n";
    }
    return mostDifferentGenotypeIdx;
}

void ARK::setNoisy (float percentage){
    noisy = true;
    noisePercentage = percentage;
}
