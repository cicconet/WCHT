//
//  Image.cpp
//  PAT
//
//  This code is distributed under the MIT Licence.
//  See notice at the end of this file.
//

#include "Image.h"

static unsigned int component(png_const_bytep row,
                              png_uint_32 x,
                              unsigned int c,
                              unsigned int bit_depth,
                              unsigned int channels)
{
    png_uint_32 bit_offset_hi = bit_depth * ((x >> 6) * channels);
    png_uint_32 bit_offset_lo = bit_depth * ((x & 0x3f) * channels + c);
    
    row = (png_const_bytep)(((PNG_CONST png_byte (*)[8])row) + bit_offset_hi);
    row += bit_offset_lo >> 3;
    bit_offset_lo &= 0x07;
    
    switch (bit_depth)
    {
        case 1: return (row[0] >> (7-bit_offset_lo)) & 0x01;
        case 2: return (row[0] >> (6-bit_offset_lo)) & 0x03;
        case 4: return (row[0] >> (4-bit_offset_lo)) & 0x0f;
        case 8: return row[0];
        case 16: return (row[0] << 8) + row[1];
        default:
            fprintf(stderr, "pngpixel: invalid bit depth %u\n", bit_depth);
            exit(1);
    }
}

float gray_level(png_structp png_ptr,
                 png_infop info_ptr,
                 png_const_bytep row,
                 png_uint_32 x)
{
    float level = 0.0;
    
    PNG_CONST unsigned int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    switch (png_get_color_type(png_ptr, info_ptr))
    {
        case PNG_COLOR_TYPE_GRAY: {
            level = (float)component(row, x, 0, bit_depth, 1)/255.0;
            break;
        }
        case PNG_COLOR_TYPE_RGB: {
            float levelR = (float)component(row, x, 0, bit_depth, 3)/255.0;
            float levelG = (float)component(row, x, 1, bit_depth, 3)/255.0;
            float levelB = (float)component(row, x, 2, bit_depth, 3)/255.0;
            level = 0.21*levelR + 0.71*levelG + 0.07*levelB;
            break;
        }
        default:
            png_error(png_ptr, "invalid color type: should be gray or rgb (with no alpha channel)");
            break;
    }
    return level;
}

void Image::set_up_with_path(const char * path)
{
    FILE *f = fopen(path, "rb");
    volatile png_bytep row = NULL;
    if (f != NULL) {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                     NULL, NULL, NULL);
        if (png_ptr != NULL) {
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr != NULL) {
                if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                    // png_uint_32 width, height;
                    int bit_depth, color_type, interlace_method,
                    compression_method, filter_method;
                    png_bytep row_tmp;
                    png_init_io(png_ptr, f);
                    png_read_info(png_ptr, info_ptr);
                    row = (volatile png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
                    row_tmp = row;
                    if (png_get_IHDR(png_ptr, info_ptr, (png_uint_32 *)&width, (png_uint_32 *)&height,
                                     &bit_depth, &color_type, &interlace_method,
                                     &compression_method, &filter_method)) {
                        int passes, pass;
                        switch (interlace_method) {
                            case PNG_INTERLACE_NONE:
                                passes = 1;
                                break;
                                
                            case PNG_INTERLACE_ADAM7:
                                passes = PNG_INTERLACE_ADAM7_PASSES;
                                break;
                                
                            default:
                                png_error(png_ptr, "pngpixel: unknown interlace");
                        }
                        data = (float *)calloc(width*height, sizeof(float));
                        png_start_read_image(png_ptr);
                        for (pass=0; pass<passes; ++pass) {
                            png_uint_32 ystart, xstart, ystep, xstep;
                            png_uint_32 py;
                            if (interlace_method == PNG_INTERLACE_ADAM7) {
                                if (PNG_PASS_COLS(width, pass) == 0)
                                    continue;
                                xstart = PNG_PASS_START_COL(pass);
                                ystart = PNG_PASS_START_ROW(pass);
                                xstep = PNG_PASS_COL_OFFSET(pass);
                                ystep = PNG_PASS_ROW_OFFSET(pass);
                            } else {
                                ystart = xstart = 0;
                                ystep = xstep = 1;
                            }
                            for (py = ystart; py < height; py += ystep) {
                                png_uint_32 px, ppx;
                                png_read_row(png_ptr, row_tmp, NULL);
                                for (px = xstart, ppx = 0; px < width; px += xstep, ++ppx) {
                                    float level = gray_level(png_ptr, info_ptr, row_tmp, ppx);
                                    data[py*width+px] = level;
                                } /* x loop */
                            } /* y loop */
                        } /* pass loop */
                        row = NULL;
                        png_free(png_ptr, row_tmp);
                    } else {
                        png_error(png_ptr, "pngpixel: png_get_IHDR failed");
                    }
                }
                else {
                    if (row != NULL) {
                        png_bytep row_tmp = row;
                        row = NULL;
                        png_free(png_ptr, row_tmp);
                    }
                }
                png_destroy_info_struct(png_ptr, &info_ptr);
            } else {
                fprintf(stderr, "pngpixel: out of memory allocating png_info\n");
            }
            png_destroy_read_struct(&png_ptr, NULL, NULL);
        } else {
            fprintf(stderr, "pngpixel: out of memory allocating png_struct\n");
        }
    } else {
        fprintf(stderr, "pngpixel: %s: could not open file\n", path);
    }
}

void Image::set_up_with_data(float * d, int w, int h)
{
    width = w;
    height = h;
    data = (float *)calloc(width*height, sizeof(float));
    if (d) {
        memcpy(data, d, width*height*sizeof(float));
    }
}

void Image::copy_from_image(Image image)
{
    // images should be of same size
    memcpy(data, image.data, image.width*image.height*sizeof(float));
}

void Image::save_png_to_path(const char * path)
{
    png_image image;
    memset(&image, 0, sizeof image);
    image.version = PNG_IMAGE_VERSION;
    image.format = PNG_FORMAT_GRAY;
    image.width = width;
    image.height = height;
    image.flags = 0;
    image.colormap_entries = 256;
    
    png_bytep buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
    
    if (buffer != NULL) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                buffer[i*width+j] = 255*data[i*width+j];
            }
        }
        if (!png_image_write_to_file(&image, path, 0/*convert_to_8bit*/, buffer, 0/*row_stride*/, NULL/*colormap*/)) {
            fprintf(stderr, "pngtopng: write %s: %s\n", path, image.message);
        }
        free(buffer);
    } else {
        fprintf(stderr, "pngtopng: out of memory: %lu bytes\n", (unsigned long)PNG_IMAGE_SIZE(image));
    }
}

void Image::normalize(void)
{
    float min = INFINITY;
    float max = -INFINITY;
    for (int i = 0; i < width*height; i++) {
        float v = data[i];
        if (v < min) min = v;
        if (v > max) max = v;
    }
    float range = max-min;
    if (range > 0) {
        for (int i = 0; i < width*height; i++) {
            data[i] = (data[i]-min)/range;
        }
    } else {
        for (int i = 0; i < width*height; i++) {
            data[i] -= min;
        }
    }
}

void Image::set_zero(void)
{
    for (int i = 0; i < width*height; i++) {
        data[i] = 0.0;
    }
}

void Image::clean_up(void)
{
    free(data);
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