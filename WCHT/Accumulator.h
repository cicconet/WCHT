//
//  Accumulator.h
//  WCHT
//
//  Created by Marcelo Cicconet on 3/12/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
//

#ifndef __WCHT__Accumulator__
#define __WCHT__Accumulator__

#include <iostream>
#include "Image.h"

class Accumulator {
    
public:
    void set_up(Image image, int radius, float scale, int nOrientations);
};

#endif /* defined(__WCHT__Accumulator__) */
