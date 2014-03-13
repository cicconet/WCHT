//
//  Wavelet.cpp
//  WCHT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include "Wavelet.h"

void Wavelet::set_up(int stretch, float scale, float orientation, int nPeaks)
{
    // controls width of gaussian window (default: scale)
    float sigma = scale;
    
    // orientation (in radians)
    float theta = orientation/360.0*2.0*M_PI;
    
    // controls elongation in direction perpendicular to wave
    float gamma = 1.0/(1.0+(float)stretch);
    
    // width and height of kernel
    int support = 2.5*sigma/gamma;
    width = 2*support+1;
    height = 2*support+1;
    
    // wavelength (default: 4*sigma)
    float lambda = 1.0/(float)nPeaks*4.0*sigma;
    
    // phase offset (in radians)
    float psi = 0.0;
    
    kernelR = (float *)malloc(width*height*sizeof(float));
    kernelI = (float *)malloc(width*height*sizeof(float));
    
    float sumReal = 0.0;
    float sumImag = 0.0;
    for (int x = -support; x <= support; x++) {
        for (int y = -support; y <= support; y++) {
            float xprime = cosf(theta)*x+sinf(theta)*y;
            float yprime = -sinf(theta)*x+cosf(theta)*y;
            float expfactor = expf(-0.5/(sigma*sigma)*(xprime*xprime+gamma*gamma*yprime*yprime));
            float mr = expfactor*cosf(2.0*M_PI/lambda*xprime+psi);
            float mi = expfactor*sinf(2.0*M_PI/lambda*xprime+psi);
            int row = support+x;
            int col = support+y;
            int index = row*width+col;
            kernelR[index] = mr;
            kernelI[index] = mi;
            sumReal += mr;
            sumImag += mi;
        }
    }
    
    // make mean = 0
    float offsetReal = sumReal/(width*height);
    float offsetImag = sumImag/(width*height);
    sumReal = 0.0;
    sumReal = 0.0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            kernelR[index] -= offsetReal;
            kernelI[index] -= offsetImag;
            sumReal += (kernelR[index]*kernelR[index]);
            sumImag += (kernelI[index]*kernelI[index]);
        }
    }
    
    // make norm = 1
    float denReal = sqrtf(sumReal);
    float denImag = sqrtf(sumImag);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            kernelR[index] /= denReal;
            kernelI[index] /= denImag;
        }
    }
    
    kernelV = NULL;
}

void Wavelet::prepare_to_visualize_kernel(const char * name)
{
    if (!kernelV) {
        kernelV = (float *)malloc(width*height*sizeof(float));
    }
    
    float min = INFINITY;
    float max = -INFINITY;
    
    float * kernel;
    
    if (strcmp(name, "real") == 0) {
        kernel = kernelR;
    } else if (strcmp(name, "imaginary") == 0) {
        kernel = kernelI;
    }
    for (int i = 0; i < width*height; i++) {
        if (kernel[i] < min) min = kernel[i];
        if (kernel[i] > max) max = kernel[i];
    }
    for (int i = 0; i < width*height; i++) {
        kernelV[i] = (kernel[i]-min)/(max-min);
    }
}

void Wavelet::clean_up(void)
{
    free(kernelR);
    free(kernelI);
    if (kernelV) {
        free(kernelV);
    }
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