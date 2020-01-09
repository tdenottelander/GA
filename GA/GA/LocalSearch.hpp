//
//  LocalSearch.hpp
//  GA
//
//  Created by Tom den Ottelander on 07/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef LocalSearch_hpp
#define LocalSearch_hpp

#include <stdio.h>
#include "GA.hpp"
#include "Utility.hpp"

class LocalSearch : public GA {
public:
    LocalSearch ();
    LocalSearch (FitnessFunction * fitFunc, Utility::Order localSearchOrder);
    Utility::Order localSearchOrder;
    void round() override;
    void doLocalSearch(Individual &ind, FitnessFunction * fitfunc);
    GA* clone() const override;
    std::string id() override;
};

#endif /* LocalSearch_hpp */

