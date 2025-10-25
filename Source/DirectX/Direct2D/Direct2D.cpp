#include "DirectX/Direct2D/Direct2D.h"
#include "Editor/MainWindow.h"
#include "Platform/Windows/WindowsShell.h"
#include "Object/Component.h"
#include "Devices/Mouse.h"

OpDirect2D* OpDirect2D::Handle() { return pSelf; }
OpDirect2D* OpDirect2D::pSelf = nullptr;

OpDirect2D::OpDirect2D() :
  pFactory{ NULL },
  pRenderTarget{ NULL },
  pSolidColorBrush{ NULL },
  m_ellipse{},
  m_rect{}
{
  pSelf = this;
  if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
    MessageBoxA(MainWindow::Handle()->Window(), "Failed to create factory!", "Error Message", MB_OK + MB_ICONERROR);
  else
  {
    MainWindow::Handle()->OnWindowResize().subscribe([this]() { resize(); });
  }
}

OpDirect2D::~OpDirect2D()
{
  discardGraphicsResources();
  pSelf = nullptr;
}

// Allocate Ressources on the GPU.
HRESULT OpDirect2D::createGraphicsResources()
{
  HRESULT hr{ S_OK };
  MainWindow* pMainWindow{ MainWindow::Handle() };

  if (pRenderTarget == NULL && pMainWindow != nullptr)
  {
    RECT rc;
    GetClientRect(pMainWindow->Window(), &rc);

    // Get size of window
    D2D1_SIZE_U size{ D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

    // Allocate ressources on the GPU
    hr = pFactory->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(pMainWindow->Window(), size),
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
void OpDirect2D::discardGraphicsResources()
{
  OShell::safeRelease(&pRenderTarget);
  OShell::safeRelease(&pSolidColorBrush);
}

// Window Resize
void OpDirect2D::resize()
{
  if (!pRenderTarget) return;

  RECT rc;

  GetClientRect(MainWindow::Handle()->Window(), &rc);

  D2D1_SIZE_U size{ D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

  // Update the render target size
  pRenderTarget->Resize(size);

  InvalidateRect(MainWindow::Handle()->Window(), &rc, FALSE);
}

void OpDirect2D::draw()
{
  HRESULT hr{ createGraphicsResources() };

  if (SUCCEEDED(hr))
  {
    PAINTSTRUCT ps;

    BeginPaint(MainWindow::Handle()->Window(), &ps);

    // Begin draw call
    pRenderTarget->BeginDraw();

    // Clear Render target
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    //RECT rc;
    //GetClientRect(MainWindow::Handle()->Window(), &rc);

    //float windowCenterX{ (static_cast<float>(rc.right) - rc.left) / 2 };
    //float windowCenterY{ (static_cast<float>(rc.bottom) - rc.top) / 2 };

    //m_ellipse.point = D2D1_POINT_2F(D2D1::Point2F(windowCenterX, windowCenterY));
    //m_ellipse.radiusX = 150.0f;
    //m_ellipse.radiusY = 150.0f;
    //
    //// Top left corner
    //m_rect.left = 50.0f;
    //m_rect.top = 50.0f;
    //m_rect.right = 150.0f;
    //m_rect.bottom = 100.0f;

    //pSolidColorBrush->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
    //pRenderTarget->FillRectangle(m_rect, pSolidColorBrush);
    //pRenderTarget->FillEllipse(m_ellipse, pSolidColorBrush);
    //
    //SYSTEMTIME time;
    //GetLocalTime(&time);
    //
    //float hours{ (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f) };
    //float minutes{ (360.0f / 60 ) *(time.wMinute)};
    //
    //pSolidColorBrush->SetColor(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

    //pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(minutes, D2D1::Point2F(windowCenterX, windowCenterY)));
    //pRenderTarget->DrawLine(m_ellipse.point, D2D_POINT_2F(windowCenterX, windowCenterY - 120.0f), pSolidColorBrush, 5);

    //pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(hours, D2D1::Point2F(windowCenterX, windowCenterY)));
    //pRenderTarget->DrawLine(m_ellipse.point, D2D_POINT_2F(windowCenterX, windowCenterY - 75.0f), pSolidColorBrush, 5);

    comp->positionX = static_cast<float>(Mouse::GetX());
    comp->positionY = static_cast<float>(Mouse::GetY());

    if (comp != nullptr)
    {
      pSolidColorBrush->SetColor(comp->color);
      m_ellipse.point = { comp->positionX, comp->positionY };
      m_ellipse.radiusX = comp->radius;
      m_ellipse.radiusY = comp->radius;
      pRenderTarget->FillEllipse(m_ellipse, pSolidColorBrush);
    }
    else
    {
      MessageBoxA(MainWindow::Handle()->hWindowHandle, "No reference to component.", "DEBUG", MB_OK + MB_ICONASTERISK);
    }

    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    hr = pRenderTarget->EndDraw();

    if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
    {
      discardGraphicsResources();
    }

    EndPaint(MainWindow::Handle()->Window(), &ps);
  }
}