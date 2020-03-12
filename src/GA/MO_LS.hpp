//
//  MO_LS.hpp
//  GA
//
//  Created by Tom den Ottelander on 07/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef MO_LS_hpp
#define MO_LS_hpp

#include <stdio.h>
#include <queue>
#include "GA.hpp"
#include "Utility.hpp"

class MO_LS : public GA {
public:
    MO_LS(FitnessFunction * fitfunc, Utility::Order order, bool loop);
    Utility::Order LS_order;
    bool loop;
    
    std::queue<float> scalarizationTargets;
    std::vector<std::pair<std::pair<float, float>, std::vector<float>>> LS_archive;
    
    void round() override;
    void performLocalSearch(Individual &ind, std::vector<float> scalarization);
    float getNewScalarizationTarget();
    
    GA* clone() const override;
    std::string id() override;
};

#endif /* MO_LS_hpp */
