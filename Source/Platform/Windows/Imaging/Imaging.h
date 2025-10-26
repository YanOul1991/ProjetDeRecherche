#pragma once

#include "Global/OptimEngineGlobal.h"
#include "Global/WindowsSettings.h"
#include <wincodec.h>

#ifdef DLL_LIB
  #define WIN_IMAGING_API __declspec(dllexport)
#else
  #define WIN_IMAGING_API __declspec(dllimport)
#endif

namespace op::Windows::Imaging
{
  HRESULT WIN_IMAGING_API LoadBitMapFromFile(
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