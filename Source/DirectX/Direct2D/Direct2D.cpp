#include "Direct2D.h"
//#include "Platform/Windows/WindowsShell.h"
#include "Platform/Windows/WindowsShellTools.h"

extern "C" {
  DIRECT2D_API Direct2DRenderer* CreateDirect2DRenderer() {
    return new Direct2DRenderer{};
  }
}

Direct2DRenderer::Direct2DRenderer() :
  pFactory{ nullptr },
  pRenderTarget{ nullptr },
  pSolidColorBrush{ nullptr }
{
  if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))) 
  {
    MessageBoxA(0, "Failed to create factory!", "Error Message", MB_OK + MB_ICONERROR);
  }
  MessageBoxA(0, "Created Direct2D instance!", "Direct2D module", MB_OK);
}

Direct2DRenderer::~Direct2DRenderer() {
  discardGraphicsResources();
  MessageBoxA(0, "Deleted Direct2D instance!", "Direct2D module", MB_OK);
}

// Allocate Ressources on the GPU.
HRESULT Direct2DRenderer::createGraphicsResources() {
  HRESULT hr{ S_OK };
  //MainWindow* pMainWindow{ MainWindow::Handle() };

  //if (pRenderTarget == NULL && pMainWindow != nullptr) {
  //  RECT rc;
  //  GetClientRect(pMainWindow->Window(), &rc);

  //  // Get size of window
  //  D2D1_SIZE_U size{ D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

  //  // Allocate ressources on the GPU
  //  hr = pFactory->CreateHwndRenderTarget(
  //    D2D1::RenderTargetProperties(),
  //    D2D1::HwndRenderTargetProperties(pMainWindow->Window(), size),
  //    &pRenderTarget
  //  );

  //  if (SUCCEEDED(hr)) {
  //    const D2D1_COLOR_F color{ D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f) };
  //    hr = pRenderTarget->CreateSolidColorBrush(color, &pSolidColorBrush);
  //  }
  //}
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

  //RECT rc;
  //GetClientRect(MainWindow::Handle()->Window(), &rc);
  //D2D1_SIZE_U size{ D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

  //// Update the render target size
  //pRenderTarget->Resize(size);
  //InvalidateRect(MainWindow::Handle()->Window(), &rc, FALSE);
}

// Draw Call
void Direct2DRenderer::draw() {
  HRESULT hr{ createGraphicsResources() };

  //if (SUCCEEDED(hr)) {
  //  PAINTSTRUCT ps;

  //  BeginPaint(MainWindow::Handle()->Window(), &ps);

  //  // Begin draw call
  //  pRenderTarget->BeginDraw();

  //  // Clear Render target
  //  pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

  //  pSolidColorBrush->SetColor({ D2D1::ColorF::Cyan });
  //  m_ellipse.point = { 250.0f, 250.0f };
  //  m_ellipse.radiusX = 50;
  //  m_ellipse.radiusY = 50;
  //  pRenderTarget->FillEllipse(m_ellipse, pSolidColorBrush);
  //  pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
  //  hr = pRenderTarget->EndDraw();

  //  if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
  //  {
  //    discardGraphicsResources();
  //  }

  //  EndPaint(MainWindow::Handle()->Window(), &ps);
  //}
}