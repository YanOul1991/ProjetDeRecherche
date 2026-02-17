/* ======================================================================================
 *  Graphics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/OptimEngine.h"
#include "Core/System/Application.h"

class CORE_API Graphics final
{
 public:
  static IGraphicsRHI* RHI();

 private:
  static void          initalize();
  static IGraphicsRHI* s_pActiveGraphicsRHI;
  friend Application;
};