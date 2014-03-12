//
//  Convolution.cpp
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include "Convolution.h"

void Convolution::set_up(int imageWidth, int imageHeight, int kernelWidth, int kernelHeight)
{
    int khw = floorf(kernelWidth/2.0); // kernel half width
    int khh = floorf(kernelHeight/2.0); // kernel half height
    int width = imageWidth+2*khw;
    int height = imageHeight+2*khh;
    paddedImage.set_up_with_data(NULL, width, height);
}

void Convolution::convolve(Image input, Wavelet wavelet, Image output)
{
    int khw = floorf(wavelet.width/2.0); // kernel half width
    int khh = floorf(wavelet.height/2.0); // kernel half height
    
    paddedImage.set_zero();
    for (int i = 0; i < input.height; i++) {
        for (int j = 0; j < input.width; j++) {
            paddedImage.data[(khh+i)*paddedImage.width+khw+j] = input.data[i*input.width+j];
        }
    }
    
    float accR, accI;
    int indexPadded, indexKernel;
    for (int i = 0; i < input.height; i++) {
        for (int j = 0; j < input.width; j++) {
            accR = 0.0;
            accI = 0.0;
            for (int ii = -khh; ii <= khh ; ii++) {
                for (int jj = -khw; jj <= khw; jj++) {
                    indexPadded = (khh+i+ii)*paddedImage.width+khw+j+jj;
                    indexKernel = (khh+ii)*wavelet.width+khw+jj;
                    accR += paddedImage.data[indexPadded]*wavelet.kernelR[indexKernel];
                    accI += paddedImage.data[indexPadded]*wavelet.kernelI[indexKernel];
                }
            }
            output.data[i*output.width+j] = sqrtf(accR*accR+accI*accI);
        }
    }
    
    output.normalize();
}

void Convolution::clean_up(void)
{
    paddedImage.clean_up();
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