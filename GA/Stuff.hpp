//
//  Stuff.hpp
//  GA
//
//  Created by Tom den Ottelander on 02/12/2019.
//  Copyright © 2019 Tom den Ottelander. All rights reserved.
//

#ifndef Stuff_hpp
#define Stuff_hpp

#include <stdio.h>
#include <armadillo>
#include "Python.h"
#include "Variation.hpp"
#include "Selection.hpp"
#include "SimpleGA.hpp"
#include "Utility.hpp"

namespace Stuff {
    int mainLoop ();
    void pythonScript();
    void pythonScript1();
    void pythonScript2();
    void pythonScript3();
    void pythonScript4();
    void sanityCheck();
}

#endif /* Stuff_hpp */
