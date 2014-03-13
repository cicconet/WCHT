//
//  main.cpp
//  WCHT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include <iostream>
#include "Image.h"
#include "Accumulator.h"
#include "LocMax.h"

void find_circles(void);

// to compile on Terminal, type
// g++ *.cpp -o wcht -I/usr/local/include -L/opt/local/lib -lpng

int main(int argc, const char * argv[])
{
    find_circles();
    
    return 0;
}

void find_circles(void)
{
    Image image;
    image.set_up_with_path("/Users/Cicconet/MacDevelopment/WCHT/Cells.png");
    
    Accumulator acc;
    int radius = 28; // 28 for Cells.png, 50 for C_.png
    float scale = 1.0;
    int nOrientations = 16;
    acc.set_up(image.width, image.height, radius, scale, nOrientations);
    Image accOut;
    accOut.set_up_with_data(NULL, image.width, image.height);
    acc.compute(image, accOut);
    image.save_png_to_path("/Users/Cicconet/Desktop/Input.png");
    accOut.save_png_to_path("/Users/Cicconet/Desktop/Votes.png");
    acc.clean_up();
    
    LocMax lm;
    int kernelHalfSize = 5;
    int nLocMax = 10; // maximum number of centers to find
    float threshold = 0.5;
    lm.set_up(accOut, kernelHalfSize, nLocMax, threshold);
    lm.find();
    Image centers;
    centers.set_up_with_data(image.data, image.width, image.height);
    for (int i = 0; i < image.width*image.height; i++) { centers.data[i] *= 0.25; }
    for (int i = 0; i < lm.nLocs; i++) {
        centers.data[(int)lm.locs[i].x*centers.width+(int)lm.locs[i].y] = 1.0;
    }
    centers.save_png_to_path("/Users/Cicconet/Desktop/Centers.png");
    lm.clean_up();

    accOut.clean_up();
    image.clean_up();
}

//
// Copyright (c) 2014 Marcelo Cicconet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//