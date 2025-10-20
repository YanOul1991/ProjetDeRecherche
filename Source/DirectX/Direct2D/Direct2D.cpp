#include "DirectX/Direct2D/Direct2D.h"
#include "Editor/MainWindow.h"
#include "Platform/Windows/WindowsShell.h"

Graphics::Graphics(MainWindow& hMainWindow) :
  pFactory          { NULL },
  pRenderTarget     { NULL },
  pSolidColorBrush  { NULL },
  hMainWindow       { hMainWindow },
  m_ellipse         {},
  m_rect            {}
{ 
  if(FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
    MessageBoxA(hMainWindow.Window(), "Failed to create factory!", "Error Message", MB_OK + MB_ICONERROR);
  else
  {
    MessageBoxA(hMainWindow.Window(), "Factory class created", "DEBUG", MB_OK);
    hMainWindow.OnWindowResize().subscribe([this]() { resize(); });
  }
}

Graphics::~Graphics()
{ 
  discardGraphicsResources(); 
}

// Allocate Ressources on the GPU.
HRESULT Graphics::createGraphicsResources()
{
  HRESULT hr { S_OK };

  if(pRenderTarget == NULL)
  {
    RECT rc;
    GetClientRect(hMainWindow.Window(), &rc);

    // Get size of window
    D2D1_SIZE_U size { D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

    // Allocate ressources on the GPU
    hr = pFactory->CreateHwndRenderTarget(
      D2D1::RenderTargetProperties(),
      D2D1::HwndRenderTargetProperties(hMainWindow.Window(), size),
      &pRenderTarget
    );

    if(SUCCEEDED(hr))
    {
      const D2D1_COLOR_F color{ D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f) };
      hr = pRenderTarget->CreateSolidColorBrush(color, &pSolidColorBrush);
    }

     MessageBoxA(hMainWindow.Window(), "Created graphic resources", "DEBUG", MB_OK);
  }
  return hr;
}

// Cleans up allocated GPU resources.
void Graphics::discardGraphicsResources()
{
  OShell::safeRelease(&pRenderTarget);
  OShell::safeRelease(&pSolidColorBrush);
}

// Window Resize
void Graphics::resize()
{
  if(pRenderTarget != NULL)
  {
    RECT rc;
    GetClientRect(hMainWindow.Window(), &rc);

    D2D1_SIZE_U size { D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top) };

    // Update the render target size
    pRenderTarget->Resize(size);
    InvalidateRect(hMainWindow.Window(), &rc, FALSE);
  }
}

void Graphics::draw()
{
  HRESULT hr { createGraphicsResources() };

  if(SUCCEEDED(hr))
  {
    PAINTSTRUCT ps;

    BeginPaint(hMainWindow.Window(), &ps);

    // Begin draw call
    pRenderTarget->BeginDraw();

    // Clear Render target
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    RECT rc;
    GetClientRect(hMainWindow.Window(), &rc);

    float windowCenterX{ (static_cast<float>(rc.right) - rc.left) / 2 };
    float windowCenterY{ (static_cast<float>(rc.bottom) - rc.top) / 2 };
    
    m_ellipse.point = D2D1_POINT_2F(D2D1::Point2F(windowCenterX, windowCenterY));
    m_ellipse.radiusX = 150.0f;
    m_ellipse.radiusY = 150.0f;
    
    // Top left corner
    m_rect.left = 50.0f;
    m_rect.top = 50.0f;
    m_rect.right = 150.0f;
    m_rect.bottom = 100.0f;

    
    pSolidColorBrush->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));
    pRenderTarget->FillRectangle(m_rect, pSolidColorBrush);
    pRenderTarget->FillEllipse(m_ellipse, pSolidColorBrush);
    
    
    SYSTEMTIME time;
    GetLocalTime(&time);
    
    float hours{ (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f) };
    float minutes{ (360.0f / 60 ) *(time.wMinute)};
    
    pSolidColorBrush->SetColor(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));

    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(minutes, D2D1::Point2F(windowCenterX, windowCenterY)));
    pRenderTarget->DrawLine(m_ellipse.point, D2D_POINT_2F(windowCenterX, windowCenterY - 120.0f), pSolidColorBrush, 5);

    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(hours, D2D1::Point2F(windowCenterX, windowCenterY)));
    pRenderTarget->DrawLine(m_ellipse.point, D2D_POINT_2F(windowCenterX, windowCenterY - 75.0f), pSolidColorBrush, 5);

    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    hr = pRenderTarget->EndDraw();

    if(FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
    {
      discardGraphicsResources();
    }

    EndPaint(hMainWindow.Window(), &ps);
  }
}