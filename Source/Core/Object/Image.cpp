/* ======================================================================================
 *  Image.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Color/Color.h"
#include "Image.h"

Image::Image() :
	width{ 0 },
	height{ 0 },
	pixels{ nullptr }
{
}

Image::~Image()
{
	delete[] pixels;
}
