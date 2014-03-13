//
//  LocMax.cpp
//  WCHT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include "LocMax.h"
#include "Image.h"

void LocMax::set_up(Image image, int kernelHalfSize, int nLocMax, float threshold)
{
    nlm = nLocMax;
    thr = threshold;
    
    locs = (Point2D *)malloc(nlm*sizeof(Point2D));
    
    im.set_up_with_data(NULL, image.width, image.height);
    GaussKernel kernel = set_up_kernel(kernelHalfSize);
    
    blur(image, kernel, im);
    im.normalize();
    
    clean_up_kernel(kernel);
}

void LocMax::clean_up(void)
{
    free(locs);
    im.clean_up();
}

void LocMax::find(void)
{
    // threshold should be in [0,1]
    std::vector<float> values;
    std::vector<Point2D> locations;
    for (int i = 1; i < im.height-1; i++) {
        for (int j = 1; j < im.width; j++) {
            float v = im.data[i*im.width+j];
            if (v > thr) {
                float vE = im.data[i*im.width+j+1];
                float vW = im.data[i*im.width+j-1];
                float vN = im.data[(i-1)*im.width+j];
                float vS = im.data[(i+1)*im.width+j];
                if (v > vE && v > vW && v > vN && v > vS) {
                    values.insert(values.end(), v);
                    locations.insert(locations.end(), Point2DMake(i, j));
                }
            }
        }
    }
    nLocs = nlm;
    if (values.size() < nLocs) {
        nLocs = (int)values.size();
    }
    for (int i = 0; i < nLocs; i++) {
        float max = -INFINITY;
        float jMax = 0;
        for (int j = 0; j < values.size(); j++) {
            float value = values.at(j);
            if (value > max) {
                max = value;
                jMax = j;
            }
        }
        locs[i] = Point2DMake(locations.at(jMax).x, locations.at(jMax).y);
        values.at(jMax) = 0.0;
    }
}

void LocMax::blur(Image input, GaussKernel kernel, Image output)
{
    int khs = floorf(kernel.size/2.0); // kernel halfsize
   
    Image padIm;
    padIm.set_up_with_data(NULL, input.width+2*khs, input.height+2*khs);
    
    for (int i = 0; i < input.height; i++) {
        for (int j = 0; j < input.width; j++) {
            padIm.data[(khs+i)*padIm.width+khs+j] = input.data[i*input.width+j];
        }
    }
    
    float acc;
    int indexPadded, indexKernel;
    for (int i = 0; i < input.height; i++) {
        for (int j = 0; j < input.width; j++) {
            acc = 0.0;
            for (int ii = -khs; ii <= khs ; ii++) {
                for (int jj = -khs; jj <= khs; jj++) {
                    indexPadded = (khs+i+ii)*padIm.width+khs+j+jj;
                    indexKernel = (khs+ii)*kernel.size+khs+jj;
                    acc += padIm.data[indexPadded]*kernel.data[indexKernel];
                }
            }
            output.data[i*output.width+j] = acc;
        }
    }
    
    output.normalize();
    padIm.clean_up();
}

GaussKernel LocMax::set_up_kernel(int halfSize)
{
    int size = 2*halfSize+1; // size should be odd
    
    GaussKernel kernel;
    kernel.size = size;
    kernel.data = (float *)malloc(size*size*sizeof(float));
    
    int i0 = size/2;
    int j0 = i0;
    float sd = (float)size/4.0; // standard deviation
    float variance = sd*sd;
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            float xdist = i-i0;
            float ydist = j-j0;
            float value = expf(-0.5*(xdist*xdist+ydist*ydist)/variance);
            kernel.data[i*size+j] = value;
            sum += value;
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            kernel.data[i*size+j] /= sum;
        }
    }
    
//    Image image;
//    image.set_up_with_data(kernel.data, size, size);
//    image.normalize();
//    image.save_png_to_path("/Users/Cicconet/Desktop/Kernel.png");
//    image.clean_up();
    
    return kernel;
}

void LocMax::clean_up_kernel(GaussKernel kernel)
{
    free(kernel.data);
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