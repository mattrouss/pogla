//************************************************
//*                                              *
//*   TP 1&2    (c) 2017 J. FABRIZIO             *
//*                                              *
//*                               LRDE EPITA     *
//*                                              *
//************************************************

#include "image.h"
#include <cstdlib>
#include <math.h>

#include <iostream>

namespace io {

gray8_image::gray8_image(int _width, int _height) {
    width = _width;
    height = _height;

    length = width*height;
    pixels = (GRAY8)aligned_alloc(TL_IMAGE_ALIGNMENT, length);
}

gray8_image::gray8_image(const gray8_image &img): width(img.width), height(img.height) {
    length = width*height;
    pixels = (GRAY8)aligned_alloc(TL_IMAGE_ALIGNMENT, length);
    for (int i = 0; i < length; i++)
        pixels[i] = img.pixels[i];

}

    gray8_image::~gray8_image() {
  free(pixels);
}

const GRAY8& gray8_image::get_buffer() const {
    return pixels;
}

GRAY8& gray8_image::get_buffer() {
    return pixels;
}
rgb24_image::rgb24_image(int _width, int _height) {
    width = _width;
    height = _height;

    length = width * height * 3;
    pixels = (RGB8)aligned_alloc(TL_IMAGE_ALIGNMENT, length);
}

rgb24_image::~rgb24_image() {
  free(pixels);
}

const RGB8& rgb24_image::get_buffer() const {
    return pixels;
}

RGB8& rgb24_image::get_buffer() {
    return pixels;
}

}
