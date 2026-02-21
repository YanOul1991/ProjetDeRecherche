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

  int32 width;
  int32 height;

  op::color::SColor* pixels{};
};