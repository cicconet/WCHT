//
//  Accumulator.h
//  WCHT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#ifndef __WCHT__Accumulator__
#define __WCHT__Accumulator__

#include <iostream>
#include "Image.h"
#include "Wavelet.h"
#include "Convolution.h"

class Accumulator {
    int * row0s;
    int * row1s;
    int * col0s;
    int * col1s;
    int row0;
    int col0;
    float wScale;
    int nOrient;
    Convolution conv;
    Image convIn;
    Image convOut;
public:
    Image acc;
    void set_up(int imWidth, int imHeight, int radius, float scale, int nOrientations);
    void compute(Image image, Image accOut);
    void clean_up(void);
};

#endif /* defined(__WCHT__Accumulator__) */

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