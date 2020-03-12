//
//  NK.cpp
//  GA
//
//  Created by Tom den Ottelander on 14/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#include "NK.hpp"

using namespace std;
using namespace nlohmann;

//NK::NK() : FitnessFunction(-1, -1, getProblemType(3)), blocksize(3), wraparound(false), alphabetSize(3), numBlocks(8 - 3 + 1), tableIndex(0) {
//    totalProblemLength = 8;
////    createTable();
//}

NK::NK(int problemLength, int blocksize, bool wraparound, int alphabetsize) :
    FitnessFunction(getProblemType(alphabetsize)),
    blocksize(blocksize),
    wraparound(wraparound),
    alphabetSize(alphabetsize),
    numBlocks(problemLength - blocksize + 1),
    tableIndex(0)
    {
        totalProblemLength = problemLength;
        loadTable(tableIndex);
//        createTable();
}

void NK::clear(){
    FitnessFunction::clear();
    tableIndex = (tableIndex + 1) % 50;
    loadTable(tableIndex);
//    cout << "New table index: " << tableIndex << " with opt " << optimum << endl;
}

//void NK::createTable(){
//    cout << "Creating lookup table for NK function. This may take a while.";
//    int alphabetSize = problemType->alphabet.size();
//    int numSubFunctions = totalProblemLength - blocksize + 1;
//    int numPossiblitiesPerSubfunction = pow(alphabetSize, blocksize);
//    vector<vector<float>> fitnessLookup;
//    fitnessLookup.reserve(numSubFunctions);
//
//    for (int i = 0; i < numSubFunctions; i++){
////        cout << "fitnessPerBlock " << i << ":" << endl;
//        vector<float> fitnessPerBlock;
//        fitnessPerBlock.reserve(numPossiblitiesPerSubfunction);
//        for (int hash = 0; hash < numPossiblitiesPerSubfunction; hash++){
//            fitnessPerBlock.push_back(Utility::getRand());
//            uvec block = HashingFunctions::decode(hash, blocksize, alphabetSize);
////            cout << hash << " -> " << Utility::genotypeToString(block) << ": " << fitnessPerBlock[hash] << endl;
//        }
//        fitnessLookup.push_back(fitnessPerBlock);
//        cout << ".";
//    }
//
////    cout << endl;
////    cout << "lookupTable: " << endl;
//    float opt = -1;
//    int numPossibleGenotypes = pow(alphabetSize, totalProblemLength);
//    for (int hash = 0; hash < numPossibleGenotypes; hash++){
//        float fitness = 0;
//        uvec genotype = HashingFunctions::decode(hash, totalProblemLength, alphabetSize);
////        cout << hash << " -> " << Utility::genotypeToString(genotype) << ": ";
//        for (int i = 0; i < numSubFunctions; i++){
//            uvec block = genotype.subvec(i, i+2);
//            int subhash = HashingFunctions::hash(block, alphabetSize);
//            fitness += fitnessLookup[i][subhash];
////            cout << fitnessLookup[i][subhash] << "+";
//        }
////        cout << "   = " << fitness << endl;
//        lookupTable[Utility::genotypeToString(genotype)] = fitness;
////        cout << "fitness: " << fitness << " > " << "opt: " << opt << " ? " << "result=" << (fitness > opt) << endl;
//        if (fitness > opt){
////            cout << "New top fitness: " << fitness << endl;
//            opt = fitness;
//        }
//        if (hash % (numPossibleGenotypes/10) == 0){
//            cout << ".";
//        }
//    }
//    optimum = opt;
//
//    cout << "\nOptimum is " << opt << endl;
//}

vector<float> NK::evaluate(Individual &ind){
    vector<float> fitness (1, 0);
    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++){
        vector<int> block (ind.genotype.begin() + blockIndex, ind.genotype.begin() + blockIndex + blocksize - 1 );
        fitness[0] += evaluateBlock(blockIndex, block);
    }
    
    ind.fitness[0] = fitness[0];
    evaluationProcedure(ind);
    
    return fitness;
}

float NK::evaluateBlock(int blockIndex, vector<int> block){
    return table["block" + to_string(blockIndex)][HashingFunctions::hash(block, alphabetSize)];
}

ProblemType* NK::getProblemType(int alphabetsize){
    vector<int> alphabet (alphabetsize, 0);
    for (int i = 0; i < alphabetsize; i++){
        alphabet[i] = i;
    }
    return new AlphabetProblemType(alphabet);
}

void NK::loadTable(int tableIndex){
    string filename = getFileName(blocksize, wraparound, alphabetSize, 50);
    json data = Utility::readJSON(folderprefix + filename);
    table = data["table" + to_string(tableIndex)];
    float opt = table["optima"][to_string(totalProblemLength)];
    setOptimum(opt);
}

void NK::display() {
    cout << "NK fitness function with id=[" << id() << "]" << endl;
}

string NK::id() {
    string result = "NK-l=" + to_string(totalProblemLength) + "-bs=" + to_string(blocksize) + "-w=";
    if(wraparound){
        result += "t";
    } else {
        result += "f";
    }
    result += ("-as=" + to_string(alphabetSize));
    return result;
}

FitnessFunction* NK::clone() const {
    return new NK(static_cast<const NK&>(*this));
}

void NK::setLength(int length){
    totalProblemLength = length;
}

void NK::generateTables(int maxProblemLength, int blocksize, bool wraparound, int alphabetSize, int amountOfTables){
    cout << "Creating lookup tables for NK function. This may take a while.";
    json mainJson;
    
    // Calculate fitness values for all tables
    for (int table = 0; table < amountOfTables; table++){
        cout << "Table " << table << " subfuncs";
        json tableJson;
        int numSubFunctions = maxProblemLength - blocksize + 1;
        int numPossiblitiesPerSubfunction = pow(alphabetSize, blocksize);
        vector<vector<float>> fitnessLookup;
        fitnessLookup.reserve(numSubFunctions);
        
        // Calculate fitness values for each block
        for (int i = 0; i < numSubFunctions; i++){
            vector<float> fitnessPerBlock;
            fitnessPerBlock.reserve(numPossiblitiesPerSubfunction);
            
            nlohmann::json blockJson;
            
            // Calculate the fitness value for every combination in this block
            for (int hash = 0; hash < numPossiblitiesPerSubfunction; hash++){
                float fitness = Utility::getRand();
                blockJson[hash] = fitness;
                fitnessPerBlock.push_back(fitness);
            }
            tableJson["block" + to_string(i)] = blockJson;
            fitnessLookup.push_back(fitnessPerBlock);
            
//            if((numSubFunctions % (i/5)) == 0)
//                cout << ".";
        }
        
        // FIND OPTIMA FOR EACH PROBLEM SIZE
        json optimaJson;
        cout << "opts";
        for (int probsize = blocksize; probsize < maxProblemLength; probsize++){
            float opt = getOptimum(tableJson, probsize, blocksize, wraparound, alphabetSize);
            optimaJson[to_string(probsize)] = opt;
            cout << ".";
        }
        tableJson["optima"] = optimaJson;
        mainJson["table" + to_string(table)] = tableJson;
        cout << endl;
        
        string filename = getFileName(blocksize, wraparound, alphabetSize, amountOfTables);
        Utility::write(mainJson.dump(), folderprefix, filename);
    }
    
    string filename = getFileName(blocksize, wraparound, alphabetSize, amountOfTables);
    Utility::write(mainJson.dump(), folderprefix, filename);
}

string NK::getFileName(int blocksize, int wraparound, int alphabetSize, int amountOfTables){
    return ("tables_b=" + to_string(blocksize) + "_w=" + to_string(wraparound) + "_as=" + to_string(alphabetSize) +"_t=" + to_string(amountOfTables) + ".json");
}


float NK::getOptimum(json table, int problemLength, int blocksize, bool wraparound, int alphabetSize){
    float opt = -1;
    int numPossibleGenotypes = pow(alphabetSize, problemLength);
    int numSubFunctions = problemLength - blocksize + 1;
    
    for (int hash = 0; hash < numPossibleGenotypes; hash++){
        float fitness = 0;
        vector<int> genotype = HashingFunctions::decode(hash, problemLength, alphabetSize);
        for (int i = 0; i < numSubFunctions; i++){
            vector<int> block (genotype.begin() + i, genotype.begin() + i + (blocksize - 1));
            int subhash = HashingFunctions::hash(block, alphabetSize);
            fitness = fitness + float(table["block" + to_string(i)][subhash]);
        }
        if (fitness > opt){
            opt = fitness;
        }
    }
    return opt;
}

//void NK::generateTables(int maxProblemLength, int blocksize, bool wraparound, int alphabetSize, int amountOfTables){
//    cout << "Creating lookup table for NK function. This may take a while.";
//    json mainJson;
//    for (int table = 0; table < amountOfTables; table++){
//        json tableJson;
//        int numSubFunctions = maxProblemLength - blocksize + 1;
//        int numPossiblitiesPerSubfunction = pow(alphabetSize, blocksize);
//        vector<vector<float>> fitnessLookup;
//        fitnessLookup.reserve(numSubFunctions);
//
//        for (int i = 0; i < numSubFunctions; i++){
//            //        cout << "fitnessPerBlock " << i << ":" << endl;
//            vector<float> fitnessPerBlock;
//            fitnessPerBlock.reserve(numPossiblitiesPerSubfunction);
//
//            nlohmann::json blockJson;
//
//            for (int hash = 0; hash < numPossiblitiesPerSubfunction; hash++){
//                float fitness = Utility::getRand();
//                blockJson[hash] = fitness;
//                fitnessPerBlock.push_back(fitness);
////                uvec block = HashingFunctions::decode(hash, blocksize, alphabetSize);
////                            cout << hash << " -> " << Utility::genotypeToString(block) << ": " << fitnessPerBlock[hash] << endl;
//            }
//            tableJson["block" + to_string(i)] = blockJson;
//            fitnessLookup.push_back(fitnessPerBlock);
//    //        cout << ".";
//
//        }
//
//        // FIND OPTIMA FOR EACH PROBLEM SIZE
//        json optimaJson;
//        for (int probsize = blocksize; probsize < maxProblemLength; probsize++){
////            float opt = -1;
////            int numPossibleGenotypes = pow(alphabetSize, maxProblemLength);
////
////            for (int hash = 0; hash < numPossibleGenotypes; hash++){
////                float fitness = 0;
////                uvec genotype = HashingFunctions::decode(hash, probsize, alphabetSize);
////                cout << hash << " -> " << Utility::genotypeToString(genotype) << ": ";
////                for (int i = 0; i < numSubFunctions; i++){
////                    uvec block = genotype.subvec(i, i+(blocksize - 1));
////                    int subhash = HashingFunctions::hash(block, alphabetSize);
////                    fitness += fitnessLookup[i][subhash];
//////                    cout << fitnessLookup[i][subhash] << "+";
////                }
////                //        cout << "   = " << fitness << endl;
//////                fitness[Utility::genotypeToString(genotype)] = fitness;
////                //        cout << "fitness: " << fitness << " > " << "opt: " << opt << " ? " << "result=" << (fitness > opt) << endl;
////                if (fitness > opt){
////                    //            cout << "New top fitness: " << fitness << endl;
////                    opt = fitness;
////                }
////                if (hash % (numPossibleGenotypes/10) == 0){
////                    cout << ".";
////                }
////            }
//            float opt = getOptimum(tableJson, probsize, blocksize, wraparound, alphabetSize);
//            optimaJson[to_string(probsize)] = opt;
//        }
//        tableJson["optima"] = optimaJson;
//        mainJson["table" + to_string(table)] = tableJson;
//    }
//
//    string filename = "tables_b=" + to_string(blocksize) + "_w=" + to_string(wraparound) + "_as=" + to_string(alphabetSize) + ".json";
//    //    cout << endl;
//    //    cout << "lookupTable: " << endl;
//
//
//    Utility::write(mainJson.dump(), folderprefix, filename);
//
//
////    cout << "\nOptimum is " << opt << endl;
// }
//
//float NK::getOptimum(json table, int problemLength, int blocksize, bool wraparound, int alphabetSize){
//    float opt = -1;
//    int numPossibleGenotypes = pow(alphabetSize, problemLength);
//    int numSubFunctions = problemLength - blocksize + 1;
//
//    for (int hash = 0; hash < numPossibleGenotypes; hash++){
//        float fitness = 0;
//        uvec genotype = HashingFunctions::decode(hash, problemLength, alphabetSize);
////        cout << hash << " -> " << Utility::genotypeToString(genotype) << ": ";
//        for (int i = 0; i < numSubFunctions; i++){
//            uvec block = genotype.subvec(i, i+(blocksize - 1));
//            int subhash = HashingFunctions::hash(block, alphabetSize);
////            fitness += fitnessLookup[i][subhash];
//            fitness = fitness + float(table["block" + to_string(i)][subhash]);
//            //                    cout << fitnessLookup[i][subhash] << "+";
//        }
//        //        cout << "   = " << fitness << endl;
//        //                fitness[Utility::genotypeToString(genotype)] = fitness;
//        //        cout << "fitness: " << fitness << " > " << "opt: " << opt << " ? " << "result=" << (fitness > opt) << endl;
//        if (fitness > opt){
//            //            cout << "New top fitness: " << fitness << endl;
//            opt = fitness;
//        }
//        if (hash % (numPossibleGenotypes/10) == 0){
//            cout << ".";
//        }
//    }
//    return opt;
//}
