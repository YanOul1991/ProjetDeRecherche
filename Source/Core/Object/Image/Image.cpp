/* ======================================================================================
 *  Image.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Object/Image/Image.h"

#include "Core/Types/Color.h"

Image::Image() :
    width{0},
    height{0},
    pixels{nullptr} {
}

Image::~Image() {
  delete[] pixels;
}
