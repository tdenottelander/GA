//
//  main.cpp
//  GA
//
//  Created by Tom den Ottelander on 19/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#include <random>
#include <iostream>
#include <armadillo>
#include <nlohmann/json.hpp>
//#include <Python.h>
#include "Individual.hpp"
#include "Selection.hpp"
#include "Variation.hpp"
#include "FitnessFunction.hpp"
#include "Trap.hpp"
#include "Utility.hpp"
#include "RoundSchedule.hpp"
#include "GA.hpp"
#include "GOM.hpp"
#include "SimpleGA.hpp"
//#include "Stuff.hpp"
#include "ProblemType.hpp"
#include "LearnedLTFOS.hpp"
#include <stdlib.h>

using namespace std;
using namespace arma;
using namespace Utility;
using nlohmann::json;

// The random generator used throughout the whole application
mt19937 rng(1234);
// A distribution that is used to get values between 0.0 and 1.0 by use of the rng defined above
uniform_real_distribution<float> dist(0.0, 0.9999);

const string projectsDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/";
const string dataDir = "/Users/tomdenottelander/Stack/#CS_Master/Afstuderen/projects/GA/data/";

void roundSchedule(){
    json main_json;

    // Set to -1 if it should not be a stopping condition
    int maxRounds = -1;
    int maxSeconds = -1;
    int maxPopSizeLevel = 100;
    int maxEvaluations = 10000;
    int interval = 4;
    int repetitions = 1;
    int maxProblemExponent = 5;
    
    main_json["maxPopSizeLevel"] = maxPopSizeLevel;
    main_json["maxRounds"] = maxRounds;
    main_json["maxSeconds"] = maxSeconds;
    main_json["maxEvaluations"] = maxEvaluations;
    main_json["repetitions"] = repetitions;
    main_json["maxProblemExponent"] = maxProblemExponent;
    main_json["interleavedRoundInterval"] = interval;
    
    FitnessFunction * fit = new Trap(3);
    main_json["fitnessFunction"] = fit->id();
    Selection * sel = new TournamentSelection(2);
    
    vector<GA*> gaList = {
        new GOM(fit, new LearnedLT_FOS(), true),
        new SimpleGA(fit, new UnivariateCrossover(), sel),
        new SimpleGA(fit, new OnePointCrossover(), sel),
        new GOM(fit, new Univariate_FOS(), true),
        new GOM(fit, new UnivariateOrdered_FOS(), true),
        new GOM(fit, new IncrementalLT_FOS(), true),
        new GOM(fit, new IncrementalLT_Univariate_FOS(), true)
    };
     
    
    json experiments;
    bool breakOutOfProblemSize = false;
    for(GA* ga : gaList){
        json setting;
        for(int i = 1; i < maxProblemExponent; i++){
            int problemSize = pow(2,i);
            json prob_json;
            for(int rep = 0; rep < repetitions; rep++){
                RoundSchedule rs(maxRounds, maxPopSizeLevel, maxSeconds, maxEvaluations, interval);
                rs.initialize(ga, problemSize);
                json result = rs.run();
                prob_json[to_string(rep)] = result;
                cout << "rep" << rep
                << " ga=" << ga->id()
                << " l=" << problemSize
                << " success=" << result.at("success")
                << " time=" << result.at("timeTaken")
                << " evaluations=" << result.at("evaluations") << endl;
                if(result.at("stoppingCondition") == "maxTimeExceeded"){
                    cout << "Max time exceeded, not starting anymore runs" << endl;
                    breakOutOfProblemSize = true;
                    break;
                } else if(result.at("stoppingCondition") == "maxEvaluationsExceeded"){
                    cout << "Max evaluations exceeded, not starting anymore runs" << endl;
                    breakOutOfProblemSize = true;
                    break;
                }
            }
            cout << endl;
            if(breakOutOfProblemSize){
                breakOutOfProblemSize = false;
                break;
            } else {
                setting[to_string(problemSize)] = prob_json;
            }
        }
        experiments[ga->id()] = setting;
    }
    
    main_json["experiments"] = experiments;
    write(main_json.dump(), dataDir);

}



int main(int argc, const char * argv[]) {
    
//    char mypath[]="PYTHONHOME=/Users/tomdenottelander/miniconda3/envs/myenv/";
//    putenv( mypath );
    
//    std::cout << "PYTHONHOME = " << getenv("PYTHONHOME") << std::endl;

    
    roundSchedule();
    
//    Stuff::pythonSimpleFunction();
//    Stuff::pythonArgumentFunction1();
//    Stuff::pythonArgumentFunction2();
    
//    Stuff::pythonLink();
    
    return 0;
}


