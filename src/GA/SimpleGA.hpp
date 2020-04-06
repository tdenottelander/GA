//
//  SimpleGA.hpp
//  GA
//
//  Created by Tom den Ottelander on 28/11/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef SimpleGA_hpp
#define SimpleGA_hpp

#include <stdio.h>
#include <vector>
#include "GA.hpp"

class SimpleGA : public GA {
public:
    SimpleGA (FitnessFunction * fitFunc, Variation * var, Selection * sel);
    
    void round() override;
    
    void roundPOVariationSelection();
    
    GA* clone() const override;
    
    std::string id() override;
    
    void mutate(std::vector<Individual> &pop);
};

#endif /* SimpleGA_hpp */
