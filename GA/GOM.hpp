//
//  GOM.hpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef GOM_hpp
#define GOM_hpp

#include <stdio.h>
#include "GA.hpp"
#include "FOS.hpp"
#include "Variation.hpp"

class GOM : public GA {
public:
    FOS *fos;
    GOM_Variation gom_variation;

    GOM (FitnessFunction * fitfunc, FOS *fos, bool forcedImprovement);

    void initialize() override;

    void round() override;
    
    GA* clone() const override;
    
    void setProblemLength (int length) override;
    
    std::string id() override;
};
    
#endif /* GOM_hpp */
