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
//#include "Selection.hpp"
//#include "Variation.hpp"
//#include "FitnessFunction.hpp"
//#include "Individual.hpp"

class GOM : public GA {
public:
    FOS *fos;
    GOM_Variation gom_variation;

    GOM (FitnessFunction * fitfunc, FOS *fos);

    void initialize();

    void round() override;
    
    GA* clone() const override;
    
//    void setFOS(FOS * fos);
//    void setGOM_Variation(GOM_Variation * g);
    
    void setProblemLength (int length) override;
    
    std::string id() override;
};
    
#endif /* GOM_hpp */
