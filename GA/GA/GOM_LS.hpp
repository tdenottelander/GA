//
//  GOM_LS.hpp
//  GA
//
//  Created by Tom den Ottelander on 09/01/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef GOM_LS_hpp
#define GOM_LS_hpp

#include <stdio.h>
#include "GOM.hpp"
#include "LocalSearchBase.hpp"

class GOM_LS : public GOM {
public:
    GOM_LS(FitnessFunction * fitfunc, FOS *fos, LocalSearchBase * ls, bool forcedImprovement);
    
    LocalSearchBase *ls;
    
    void round() override;
    
    GA* clone() const override;
    
    std::string id() override;
};

#endif /* GOM_LS_hpp */
