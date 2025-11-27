#include "Imaging.h"
#include "Platform/Windows/WindowsShell.h"
#include "Platform/Windows/WindowsShellTools.h"

HRESULT op::Windows::Imaging::LoadBitMapFromFile(
  ID2D1RenderTarget*  pRenderTarget,
  IWICImagingFactory* pIwicFactory,
  PCWSTR              uri,
  uint32              destinationWidth,
  uint32              destinationHeight,
  ID2D1Bitmap** ppBitmap
)
{
  IWICBitmapDecoder*      pDecoder    { nullptr };
  IWICBitmapFrameDecode*  pSource     { nullptr };
  IWICStream*             pStream     { nullptr };
  IWICFormatConverter*    pConverter  { nullptr };
  IWICBitmapScaler*       pScaler     { nullptr };

  // Load IMage from a file
  HRESULT hr = pIwicFactory->CreateDecoderFromFilename(
    uri,
    0,
    GENERIC_READ,
    WICDecodeMetadataCacheOnLoad,
    &pDecoder
  );

  // Get the First name
  if (SUCCEEDED(hr)) {
    hr = pDecoder->GetFrame(0, &pSource);
  }

  // Convert to Direct2D compatible format
  if (SUCCEEDED(hr)) {
    hr = pConverter->Initialize(
      pSource,
      GUID_WICPixelFormat32bppPRGBA,
      WICBitmapDitherTypeNone,
      0,
      0.0f,
      WICBitmapPaletteTypeMedianCut
    );
  }

  // Create the Direct2D Bitmap
  if (SUCCEEDED(hr)) {
    hr = pRenderTarget->CreateBitmapFromWicBitmap(
      pConverter,
      nullptr,
      ppBitmap
    );
  }

  // Free ressources
  op::win::shell::SafeRelease(&pDecoder);
  op::win::shell::SafeRelease(&pSource);
  op::win::shell::SafeRelease(&pStream);
  op::win::shell::SafeRelease(&pConverter);
  op::win::shell::SafeRelease(&pScaler);

  return hr;
}