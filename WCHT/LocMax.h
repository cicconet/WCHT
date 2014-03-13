//
//  LocMax.h
//  WCHT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#ifndef __Triangles__LocMax__
#define __Triangles__LocMax__

#include <iostream>
#include <vector>
#include <math.h>
#include "Image.h"

struct GaussKernel {
	int size;
    float * data;
};
typedef struct GaussKernel GaussKernel;

typedef struct Point2D {
    float x;
    float y;
} Point2D;

inline Point2D Point2DMake(float x, float y)
{
    Point2D p; p.x = x; p.y = y; return p;
}

class LocMax {
    GaussKernel set_up_kernel(int halfSize);
    void clean_up_kernel(GaussKernel kernel);
    void blur(Image input, GaussKernel kernel, Image output);
    int nlm;
    float thr;
public:
    Image im;
    Point2D * locs;
    int nLocs; // number of local maxima actually found
    void set_up(Image image, int kernelHalfSize, int nLocMax, float threshold);
    void find(void);
    void clean_up(void);
};

#endif /* defined(__Triangles__LocMax__) */

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