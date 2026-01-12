/* ======================================================================================
 *  Image.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Color/Color.h"

class CORE_API Image
{
public:
	Image();
	~Image();

	int width;
	int height;
	op::color::SColor* pixels{};
};