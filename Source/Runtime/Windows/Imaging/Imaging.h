#pragma once

#include "Global/OptimEngineGlobal.h"
#include "d2d1.h"
#include <wincodec.h>
#include <Windows.h>

namespace op::Windows::Imaging
{
  HRESULT EXPORT LoadBitMapFromFile(
    ID2D1RenderTarget*  pRenderTarget,
    IWICImagingFactory* pIwicFactory,
    PCWSTR              uri,
    uint32              destinationWidth,
    uint32              destinationHeight,
    ID2D1Bitmap**       ppBitmap
  );

  //HRESULT WIN_IMAGING_API LoadResourceBitmap(
  //  ID2D1RenderTarget*    pRenderTarget,
  //  IWICImagingFactory*   pIwicFactory,
  //  PCWSTR                resourceName,
  //  PCWSTR                resourceType,
  //  uint32                destinationWidth,
  //  uint32                destinationHeight,
  //  ID2D1Bitmap**         ppBitmap 
  //);
}