/* ======================================================================================
 *  IGraphicsModule.h:
 *      Pure virtual class interface declaring generic functionalities for all 
 *      graphics APIs.
 * 
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"

class IGraphicsModule
{
public:
  CORE_API virtual ~IGraphicsModule() {};
  CORE_API virtual void Initialize(void* _WindowHandle) = 0;
  CORE_API virtual void draw() = 0;
  CORE_API virtual void Clean() = 0;
};