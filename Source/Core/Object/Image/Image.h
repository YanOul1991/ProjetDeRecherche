/* ======================================================================================
 *  Image.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Types/Color.h"

class Image
{
public:
	CORE_API Image();
	CORE_API ~Image();

	int width;
	int height;
	op::color::SColor* pixels{};
};