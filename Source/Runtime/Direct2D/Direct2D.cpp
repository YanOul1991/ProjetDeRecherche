#include "Direct2D.h"
#include "Platform/Windows/WindowsShellTools.h"
#include <iostream>

extern "C" {
  DIRECT2D_API IRenderingModule* CreateDirect2DRenderer() {
    return new Direct2DRenderer();
  }
}

Direct2DRenderer::Direct2DRenderer() :
  pFactory          { nullptr },
  pRenderTarget     { nullptr },
  pSolidColorBrush  { nullptr },
  m_hTargetWindow   { nullptr }
{
  if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) 
  {
    MessageBoxA(0, "Failed to create factory!", "Error Message", MB_OK + MB_ICONERROR);
  }
  std::cout << "Instanciated Direct2D Renderer Class !!!\n";
}

Direct2DRenderer::~Direct2DRenderer() {
  discardGraphicsResources();
  std::cout << "Discarded Direct2D class!!!\n";
}

/*
  Interface Overrides Definitions
*/
void Direct2DRenderer::Initialize(void* _targetWindow)
{ 
  m_hTargetWindow = reinterpret_cast<HWND>(_targetWindow);
  std::cout << "Direct2D context has been initialized!!!\n";
}

/*
  Direct2D Class definitions
*/

// Allocate Ressources on the GPU.
HRESULT Direct2DRenderer::createGraphicsResources() {
  HRESULT hr{ S_OK };

  // If no render target
  if (pRenderTarget == nullptr)
  {
    std::cout << "Creating graphics resources.\n";
    // Get the target window size.
    RECT rc;
    GetClientRect(m_hTargetWindow, &rc);
    D2D1_SIZE_U size{ D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

    hr = pFactory->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(m_hTargetWindow, size),
      &pRenderTarget
    );

    if (SUCCEEDED(hr))
    {
      const D2D1_COLOR_F color{ D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f) };
      hr = pRenderTarget->CreateSolidColorBrush(color, &pSolidColorBrush);
    }
  }
  return hr;
}

// Cleans up allocated GPU resources.
void Direct2DRenderer::discardGraphicsResources() {
  op::win::shell::SafeRelease(&pRenderTarget);
  op::win::shell::SafeRelease(&pSolidColorBrush);
}

// Window Resize
void Direct2DRenderer::resize() {
  if (!pRenderTarget) return;

  RECT rc;
  GetClientRect(m_hTargetWindow, &rc);
  D2D1_SIZE_U size{ D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

  //// Update the render target size
  pRenderTarget->Resize(size);
  InvalidateRect(m_hTargetWindow, &rc, FALSE);
}

// Draw Call
void Direct2DRenderer::Draw() {
  HRESULT hr{ createGraphicsResources() };

  resize();

  if (SUCCEEDED(hr)) {
    PAINTSTRUCT ps;

    BeginPaint(m_hTargetWindow, &ps);

    // Begin draw call
    pRenderTarget->BeginDraw();

    // Clear Render target
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkCyan));
    pSolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));

    D2D1_ELLIPSE m_ellipse{ D2D1::Ellipse(
      D2D1::Point2F(300.0f, 300.0f),
      100.0f,
      100.0f
    )};

    m_ellipse.point = D2D1::Point2F(0, 0);

    pRenderTarget->FillEllipse(m_ellipse, pSolidColorBrush);

    hr = pRenderTarget->EndDraw();

    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
    {
      discardGraphicsResources();
    }

    EndPaint(m_hTargetWindow, &ps);
  }
}

void Direct2DRenderer::Clean()
{
}
