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
#include "Core/Utilities/Random/Random.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Graphics/Mesh.h"

class IGraphicsModule
{
public:
  CORE_API virtual ~IGraphicsModule() {};
  CORE_API virtual void Initialize(void* _WindowHandle) = 0;
  CORE_API virtual void draw() = 0;
  CORE_API virtual void Clean() = 0;

  /*
  CORE_API virtual SGuid* createVertexBuffer(Vertex) = 0;
  CORE_API virtual SGuid* createIndexBuffer()  = 0;
  */
};