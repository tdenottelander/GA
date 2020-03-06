//
//  LocalSearchAnalysis.hpp
//  GA
//
//  Created by Tom den Ottelander on 02/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef LocalSearchAnalysis_hpp
#define LocalSearchAnalysis_hpp

#include <stdio.h>
#include <nlohmann/json.hpp>
#include "Individual.hpp"
#include "FitnessFunction.hpp"
#include "ARK1.hpp"
#include "ARK2.hpp"
#include "ARK.hpp"


namespace LocalSearchAnalysis {
    bool findBestLocalGene(Individual &ind, int index, FitnessFunction* fitfunc);
    int localSearch(Individual &ind, FitnessFunction* fitfunc, std::string localSearchType);
    void localSearchTests(ARK* fitfunc, int runs, std::string localSearchType);
}


#endif /* LocalSearchAnalysis_hpp */
