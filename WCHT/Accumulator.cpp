//
//  Accumulator.cpp
//  WCHT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include "Accumulator.h"

void Accumulator::set_up(int imWidth, int imHeight, int radius, float scale, int nOrientations)
{
    wScale = scale;
    nOrient = nOrientations;
    
    row0s = (int *)malloc(nOrient*sizeof(int));
    row1s = (int *)malloc(nOrient*sizeof(int));
    col0s = (int *)malloc(nOrient*sizeof(int));
    col1s = (int *)malloc(nOrient*sizeof(int));
    
    row0 = radius;
    int row1 = imHeight-radius-1;
    col0 = radius;
    int col1 = imWidth-radius-1;
    
    acc.set_up_with_data(NULL, col1-col0+1, row1-row0+1);
    convIn.set_up_with_data(NULL, acc.width, acc.height);
    convOut.set_up_with_data(NULL, acc.width, acc.height);
    
    for (int i = 0; i < nOrient; i++) {
        float a = (float)i/(float)nOrient*2.0*M_PI;
        row0s[i] = row0+roundf(radius*cosf(a));
        row1s[i] = row1+roundf(radius*cosf(a));
        col0s[i] = col0+roundf(radius*sinf(a));
        col1s[i] = col1+roundf(radius*sinf(a));
    }
    
    Wavelet w;
    w.set_up(1.0, wScale, 0.0, 1);
    conv.set_up(acc.width, acc.height, w.width, w.height);
    w.clean_up();
}

void Accumulator::compute(Image image, Image accOut)
{
    acc.set_zero();
    for (int i = 0; i < nOrient; i++) {
        for (int row = row0s[i]; row <= row1s[i]; row++) {
            for (int col = col0s[i]; col <= col1s[i]; col++) {
                convIn.data[(row-row0s[i])*convIn.width+col-col0s[i]] = image.data[row*image.width+col];
            }
        }
        
        Wavelet w;
        w.set_up(1.0, wScale, (float)i/(float)nOrient*360.0, 1.0);
        conv.convolve(convIn, w, convOut);
//        char path[100];
//        sprintf(path, "/Users/Cicconet/Desktop/Image%d.png",i);
//        convOut.save_png_to_path(path);
        w.clean_up();
        
        for (int j = 0; j < acc.width*acc.height; j++) {
            acc.data[j] += convOut.data[j];
        }
    }
    acc.normalize();
    for (int row = 0; row < acc.height; row++) {
        for (int col = 0; col < acc.width; col++) {
            accOut.data[(row+row0)*accOut.width+col+col0] = acc.data[row*acc.width+col];
        }
    }
}

void Accumulator::clean_up(void)
{
    conv.clean_up();
    convIn.clean_up();
    convOut.clean_up();
    acc.clean_up();
    free(row0s);
    free(row1s);
    free(col0s);
    free(col1s);
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