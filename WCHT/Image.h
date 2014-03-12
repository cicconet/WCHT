//
//  Image.h
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#ifndef __PAT__Image__
#define __PAT__Image__

#include <iostream>
#include <math.h>
#include <png.h>

class Image {
private:

public:
    int width;
    int height;
    float * data;
    void set_up_with_path(const char * path);
    void set_up_with_data(float * d, int w, int h);
    void copy_from_image(Image image);
    void save_png_to_path(const char * path);
    void normalize(void);
    void set_zero(void);
    void clean_up(void);
};

#endif /* defined(__PAT__Image__) */

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