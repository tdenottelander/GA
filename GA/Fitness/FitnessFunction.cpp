//
//  FitnessFunction.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include "FitnessFunction.hpp"

using namespace std;
using namespace arma;
using namespace nlohmann;

extern bool storeAbsoluteConvergence;
extern bool storeUniqueConvergence;
extern bool storeTransformedUniqueConvergence;
extern bool printElitistArchiveOnUpdate;
extern bool storeDistanceToParetoFrontOnElitistArchiveUpdate;
extern nlohmann::json convergence;
extern nlohmann::json elitistArchiveJSON;
extern bool storeElitistArchive;
extern bool updateElitistArchiveOnEveryEvaluation;

/* ------------------------ Base Fitness Function ------------------------ */

FitnessFunction::FitnessFunction(vector<float> optimum, ProblemType *problemType) :
    bestIndividual(),
    optimum(optimum),
    optimumFound(false),
    maxEvaluations(-1),
    maxUniqueEvaluations(-1),
    problemType(problemType),
    totalEvaluations(0),
    totalUniqueEvaluations(0),
    totalTransformedUniqueEvaluations(0),
    uniqueSolutions(problemType->alphabet.size()),
    transformedUniqueSolutions(problemType->alphabet.size())
{
}

FitnessFunction::FitnessFunction(ProblemType *problemType) :
    bestIndividual(),
    optimumFound(false),
    maxEvaluations(-1),
    maxUniqueEvaluations(-1),
    problemType(problemType),
    totalEvaluations(0),
    totalUniqueEvaluations(0),
    totalTransformedUniqueEvaluations(0),
    uniqueSolutions(problemType->alphabet.size()),
    transformedUniqueSolutions(problemType->alphabet.size())
{
}

void FitnessFunction::clear(){
    bestIndividual = Individual();
    elitistArchive.clear();
    optimumFound = false;
    totalEvaluations = 0;
    totalUniqueEvaluations = 0;
    totalTransformedUniqueEvaluations = 0;
    uniqueSolutions = UniqueSolutions(problemType->alphabet.size());
    transformedUniqueSolutions = UniqueSolutions(problemType->alphabet.size());
    distanceToParetoFrontData.clear();
    elitistArchiveJSON.clear();
    storeElitistArchiveCount = 0;
}

// Performs additional operations like incrementing the amount of (unique) evaluations, checking whether an individual is the best so far yet and storing convergence data.
void FitnessFunction::evaluationProcedure(Individual &ind){
    checkIfBestFound(ind);
    totalEvaluations++;
//    float roundedFitness = roundf(bestIndividual.fitness * 10) / 10;  // Rounds to 3 decimal places
    float roundedFitness = bestIndividual.fitness[0];
    
    if(storeAbsoluteConvergence){
        convergence["absolute"].push_back(roundedFitness);
    }
    
    if(storeUniqueConvergence && !uniqueSolutions.contains(ind.genotype)){
        uniqueSolutions.put(ind.genotype);
        totalUniqueEvaluations++;
        convergence["unique"].push_back(roundedFitness);
    }
    
    if(storeTransformedUniqueConvergence){
        uvec transformedGenotype = transform(ind.genotype);
        if (!transformedUniqueSolutions.contains(transformedGenotype)){
            transformedUniqueSolutions.put(transformedGenotype);
            totalTransformedUniqueEvaluations++;
            convergence["transformedUnique"].push_back(roundedFitness);
        }
    }
    
    if(updateElitistArchiveOnEveryEvaluation){
        updateElitistArchive(ind);
    }
}

bool FitnessFunction::isDone(){
    if(done) {
        return true;
    }
    
    if(optimumFound || maxEvaluationsExceeded() || maxUniqueEvaluationsExceeded()){
        done = true;
    }
    
    return done;
}

bool FitnessFunction::isMO(){
    if(numObjectives > 1){
        return true;
    }
    return false;
}

// Displays the description of the fitness function
void FitnessFunction::display(){
    cout << "Base fitness function" << endl;
}

// Checks whether this individual is fitter than the best found individual so far.
// Checks whether the individual is optimal.
void FitnessFunction::checkIfBestFound(Individual &ind){
    
    if(optimumFound){
        return;
    }
    
    if(convergenceCriteria == ConvergenceCriteria::OPTIMAL_FITNESS){
        optimumFound = true;
        for (int obj = 0; obj < optimum.size(); obj++){
            if (ind.fitness[obj] < optimum[obj] || optimum[obj] == -1){
                optimumFound = false;
                break;
            }
        }
    }
    
    else if (convergenceCriteria == ConvergenceCriteria::GENOTYPE){
        if(ind.genotypeEquals(optimalGenotype)){
            optimumFound = true;
        };
    }
    
    // Set bestIndividual only for SO-problems.
    if(optimum.size() == 1 && ind.fitness[0] > bestIndividual.fitness[0]){
        bestIndividual = ind.copy();
//        cout << "this genotype: " << Utility::genotypeToString(ind.genotype) << "  opt genotype: " << Utility::genotypeToString(optimalGenotype) << endl;
    }
}


// Update the eltist archive by supplying the best front found. It adds non-dominated solution to and removes dominated solutions from the archive. Returns true if the supplied front adds any individuals to the elitist archive
bool FitnessFunction::updateElitistArchive(vector<Individual*> front){
    
    bool solutionsAdded = false;
    
    for (int i = front.size() - 1; i >= 0; i--){
        bool addToArchive = true;
        
        // Loop over every solution that is in the archive at the beginning of this method.
        for (int j = elitistArchive.size() - 1; j >= 0; j--){
            
            // Delete every solution from the archive that is dominated by this solution.
            if(front[i]->dominates(elitistArchive[j])){
//                cout << "Remove " + elitistArchive[j].toString() + " from Elitist Archive\n" << endl;
                elitistArchive.erase(elitistArchive.begin() + j);
            }
            
            // If the solution from the archive dominates this one or it is equal, then don't add this solution to the archive. Thus, remove it from the front.
            else if (elitistArchive[j].dominates(*front[i]) || front[i]->fitnessEquals(elitistArchive[j])){
                front.erase(front.begin() + i);
                addToArchive = false;
                break;
            }
        }
        if (addToArchive){
            elitistArchive.push_back(front[i]->copy());
//            cout << "Add " + front[i]->toString() + " to Elitist Archive\n" << endl;
            solutionsAdded = true;
        }
    }
    
    // Only check for convergence criteria if there are new solutions added to the elitist archive.
    if (solutionsAdded){
        if(storeElitistArchive){
            elitistArchiveJSON["archive"][storeElitistArchiveCount] = elitistArchiveToJSON();
            elitistArchiveJSON["totalEvaluations"][storeElitistArchiveCount] = totalEvaluations;
            elitistArchiveJSON["uniqueEvaluations"][storeElitistArchiveCount] = totalUniqueEvaluations;
            storeElitistArchiveCount++;
        }
        
        
        float distanceParetoToApproximation = -1;
        if (storeDistanceToParetoFrontOnElitistArchiveUpdate){
            distanceParetoToApproximation = calculateDistanceParetoToApproximation();
        }
        
        if (convergenceCriteria == ConvergenceCriteria::ENTIRE_PARETO){
            // If the true pareto front is not known, rely on enitreParetoFrontFound() functions that are implemented in child classes
            if (trueParetoFront.size() == 0){
                if(entireParetoFrontFound()){
                    optimumFound = true;
                }
            // Else, compute the distance to the front approximation and check if it is 0. (in fact if it is < 0.000001).
            } else {
                if (distanceParetoToApproximation == -1)
                    distanceParetoToApproximation = calculateDistanceParetoToApproximation();
                
                if (distanceParetoToApproximation < 0.000001){
                    optimumFound = true;
                }
            }
        }
        
        else if (convergenceCriteria == ConvergenceCriteria::EPSILON_PARETO_DISTANCE){
            if (distanceParetoToApproximation == -1)
                distanceParetoToApproximation = calculateDistanceParetoToApproximation();
            
            if (distanceParetoToApproximation <= epsilon){
                optimumFound = true;
            }
        }
        
        else if (convergenceCriteria == ConvergenceCriteria::PERCENTAGE_PARETO_COVERAGE){
            // Create method that finds intersection of elitist archive with pareto front.
            // Then compare its size with the pareto front size
            // To be implemented
        }
        
        else if (convergenceCriteria == ConvergenceCriteria::ABSOLUTE_PARETO_COVERAGE){
            // Create method that finds intersection of elitist archive with pareto front.
            // Then compare with some number
            // To be implemented
        }
        
        if (printElitistArchiveOnUpdate){
            drawElitistArchive();
        }
    }
    
    return solutionsAdded;
}

bool FitnessFunction::updateElitistArchive(vector<Individual> &front){
    vector<Individual*> pointerFront;
    pointerFront.reserve(front.size());
    for (int i = 0; i < front.size(); i++){
        pointerFront.push_back(&front[i]);
    }
    return updateElitistArchive(pointerFront);
}

// Returns true if the given individual is added to the elitist archive
bool FitnessFunction::updateElitistArchive(Individual &ind){
    return updateElitistArchive(vector<Individual*> {&ind});
}

// The elitist archive is used as approximation
float FitnessFunction::calculateDistanceParetoToApproximation(){
    if (trueParetoFront.size() == 0){
        cout << "ERROR: Wanted to calculate distance of pareto to approximation, but true Pareto front is unknown" << endl;
        storeDistanceToParetoFrontOnElitistArchiveUpdate = false;
        return -1.0f;
    }
    
    if (elitistArchive.size() == 0){
        return INFINITY;
    }
    
    float sum = 0.0f;
    
    for (int i = 0; i < trueParetoFront.size(); i++){
        float minimalDistance = INFINITY;
        for (int j = 0; j < elitistArchive.size(); j++){
            float distance = Utility::EuclideanDistance(trueParetoFront[i], elitistArchive[j].fitness);
            if (distance < minimalDistance){
                minimalDistance = distance;
                if (distance == 0.0f){
                    break;
                }
            }
        }
        sum += minimalDistance;
    }
    
    float calculatedDistance = sum / trueParetoFront.size();
    distanceToParetoFrontData.push_back(tuple<int, int, float>{totalEvaluations, totalUniqueEvaluations, calculatedDistance});
    return calculatedDistance;
}

json FitnessFunction::paretoDistanceToJSON(){
    json result;
    for (int i = 0; i < distanceToParetoFrontData.size(); i++){
        result["totalEvaluations"][i] = get<0>(distanceToParetoFrontData[i]);
        result["totalUniqueEvaluations"][i] = get<1>(distanceToParetoFrontData[i]);
        result["distance"][i] = get<2>(distanceToParetoFrontData[i]);
    }
    return result;
}

json FitnessFunction::elitistArchiveToJSON(){
    json result;
    for (int i = 0; i < elitistArchive.size(); i++){
        json array;
        array[0] = elitistArchive[i].fitness[0];
        array[1] = elitistArchive[i].fitness[1];
        result[i] = array;
    }
    return result;
}

// Override this method in specific problems. For example, do an extra check on objective values.
bool FitnessFunction::entireParetoFrontFound(){
    return elitistArchive.size() == optimalParetoFrontSize;
}

void FitnessFunction::setGenotypeChecking(uvec genotype){
    checkForGenotype = true;
    optimalGenotype = genotype;
}

// Returns the total amount of evaluations over all fitness functions.
int FitnessFunction::getTotalAmountOfEvaluations(){
    return totalEvaluations;
}

// Checks whether the maximum amount of evaluations is exceeded
bool FitnessFunction::maxEvaluationsExceeded(){
    return totalEvaluations >= maxEvaluations && maxEvaluations != -1;
}

bool FitnessFunction::maxUniqueEvaluationsExceeded(){
    return totalUniqueEvaluations >= maxUniqueEvaluations && maxUniqueEvaluations != -1;
}

// Returns the id of the fitness function
string FitnessFunction::id() {
    return "base";
}

// Sets the length and the optimum
void FitnessFunction::setLength(int length){
    totalProblemLength = length;
//    if (numObjectives == 1){
//        optimum[0] = length;
//    }
}

void FitnessFunction::setNumObjectives(int numObj){
    numObjectives = numObj;
}

// Sets the MO-problem optimum.
void FitnessFunction::setOptimum(vector<float> opt){
    optimum = opt;
}

// Sets the SO-problem optimum.
void FitnessFunction::setOptimum(float opt){
    optimum[0] = opt;
}

// Transforms the genotype (e.g. remove identity layers in ARK)
// Should be overridden in derived classes
uvec FitnessFunction::transform(uvec &genotype){
    return genotype;
}

void FitnessFunction::draw2DVisualization(vector<Individual> &population, int maxX, int maxY){
    vector<Individual*> drawList;
    drawList.reserve(population.size());
    for (int i = 0; i < population.size(); i++){
        drawList.push_back(&population[i]);
    }
    sort(drawList.begin(), drawList.end(), [](const Individual* lhs, const Individual* rhs){
        if (lhs->fitness[1] < rhs->fitness[1]){
            return true;
        } else if (lhs->fitness[1] > rhs->fitness[1]){
            return false;
        } else {
            return lhs->fitness[0] < rhs->fitness[0];
        }
    });
    int i = 0;
    string result = "";
    for (int y = 0; y < maxY; y++){
        for (int x = 0; x < maxX; x++){
            if (drawList[i]->fitness[0] == x && drawList[i]->fitness[1] == y){
                result += " o ";
            } else {
                result += " . ";
            }
            result += " ";
            while (drawList[i]->fitness[0] == x && drawList[i]->fitness[1] == y && i < drawList.size()-1){
                i++;
            }
        }
        result += "\n";
    }
    cout << result << endl;
}

void FitnessFunction::drawElitistArchive(){
    draw2DVisualization(elitistArchive, optimum[0]+1, optimum[1]+1);
}

void FitnessFunction::saveElitistArchiveToJSON(){
    json result;
    for (int i = 0; i < elitistArchive.size(); i++){
        json solution;
        solution["genotype"] = Utility::genotypeToString(elitistArchive[i].genotype);
        json fitness;
        for (int j = 0; j < numObjectives; j++){
            fitness[j] = elitistArchive[i].fitness[j];
        }
        solution["fitness"] = fitness;
        result[i] = solution;
    }
    Utility::writeRawData(result.dump(), "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/data/elitistArchiveData/", "");
}


/* ------------------------ OneMax Fitness Function ------------------------ */

OneMax::OneMax(int length) : FitnessFunction(vector<float>(1, length), getProblemType()) {}
OneMax::OneMax() : FitnessFunction(getProblemType()) {}

vector<float> OneMax::evaluate(Individual &ind) {
    vector<float> result(1, sum(ind.genotype));
    ind.fitness[0] = result[0];
    
    evaluationProcedure(ind);
    
    return result;
}

void OneMax::display() {
    cout << "OneMax fitness function" << endl;
}

string OneMax::id() {
    return "OM";
}

ProblemType* OneMax::getProblemType(){
    return new BinaryProblemType();
}

FitnessFunction* OneMax::clone() const {
    FitnessFunction* result = new OneMax(static_cast<const OneMax&>(*this));
    result->problemType = this->problemType;
    return result;
}


/* ------------------------ Leading Ones Fitness Function ------------------------ */

LeadingOnes::LeadingOnes(int length) : FitnessFunction(vector<float>(1, length), getProblemType()) {}
LeadingOnes::LeadingOnes() : FitnessFunction(getProblemType()) {}

ProblemType* LeadingOnes::getProblemType(){
    return new BinaryProblemType();
}

vector<float> LeadingOnes::evaluate(Individual &ind) {
    vector<float> result (1, 0);
    for (unsigned long i = 0; i < ind.genotype.size(); i++){
        if (ind.genotype[i] == 0){
            break;
        } else {
            result[0]++;
        }
    }
    ind.fitness[0] = result[0];
    
    evaluationProcedure(ind);
    
    return result;
}

FitnessFunction* LeadingOnes::clone() const {
    FitnessFunction* result = new LeadingOnes(static_cast<const LeadingOnes&>(*this));
    result->problemType = this->problemType;
    return result;
}

void LeadingOnes::display() {
    cout << "LeadingOnes fitness function" << endl;
}

string LeadingOnes::id() {
    return "LO";
}


/* ------------------------ Non-Binary Max Fitness Function ------------------------ */

NonBinaryMax::NonBinaryMax() : FitnessFunction(getProblemType()) {}

//TODO: FINISH IMPLEMENTATION OF NONBINARY MAX FITNESS FUNCTION

vector<float> NonBinaryMax::evaluate(Individual &ind){
    vector<float> result (1, sum(ind.genotype));
    ind.fitness[0] = result[0];
    evaluationProcedure(ind);
    return result;
}

FitnessFunction* NonBinaryMax::clone() const {
    return new NonBinaryMax(static_cast<const NonBinaryMax&>(*this));
}

void NonBinaryMax::display() {
    cout << "Non-binary Maximum fitness function" << endl;
}

string NonBinaryMax::id() {
    return "NBMax";
}

ProblemType* NonBinaryMax::getProblemType(){
    vector<int> vec = {0, 1, 2, 3, 4, 5};
    return new AlphabetProblemType(vec);
}

void NonBinaryMax::setLength(int length){
    totalProblemLength = length * 5;
    setOptimum(vector<float>(1, totalProblemLength));
}
