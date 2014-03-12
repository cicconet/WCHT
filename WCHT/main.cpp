//
//  main.cpp
//  WCHT
//
//  Created by Marcelo Cicconet on 3/12/14.
//  Copyright (c) 2014 Marcelo Cicconet. All rights reserved.
//

#include <iostream>
#include "Image.h"
#include "Accumulator.h"

void find_circles(void);

int main(int argc, const char * argv[])
{
    find_circles();
    
    return 0;
}

void find_circles(void)
{
    Image image;
    image.set_up_with_path("/Users/Cicconet/MacDevelopment/WCHT/C3.png");
    
    Accumulator acc;
    int radius = 50;
    float scale = 1.0;
    int nOrientations = 8;
    acc.set_up(image, radius, scale, nOrientations);

    image.clean_up();
}

