//
//  MO_RS.hpp
//  GA
//
//  Created by Tom den Ottelander on 26/02/2020.
//  Copyright © 2020 Tom den Ottelander. All rights reserved.
//

#ifndef MO_RS_hpp
#define MO_RS_hpp

#include <stdio.h>
#include "GA.hpp"

class MO_RS : public GA {
public:
    MO_RS (FitnessFunction * fitFunc, bool RSUSS = false);
    bool RSUSS;
    void round() override;
    void RSUSSInitialize(Individual &ind);
    GA* clone() const override;
    std::string id() override;
};

#endif /* MO_RS_hpp */
