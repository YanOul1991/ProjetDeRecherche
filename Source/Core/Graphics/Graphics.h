/**
 * Graphics.h
 */

#pragma once

#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/Graphics/Resource/GraphicResourceHandle.h"
#include "Core/OptimEngine.h"
#include "Core/System/Application.h"

class CORE_API Graphics final
{
 public:
  static IGraphicsRHI*         RHI();
  static TextureResourceHandle GetDefaultTexture();

 private:
  static void          initalize();
  static IGraphicsRHI* s_pActiveGraphicsRHI;
  friend Application;
};