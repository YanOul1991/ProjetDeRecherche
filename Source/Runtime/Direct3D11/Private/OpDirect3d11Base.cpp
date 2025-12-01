#include "OpDirect3d11Base.h"

/*
  Class constructor
*/
OpDirect3d11Base::OpDirect3d11Base() :
  m_pSwapChain{ nullptr },
  m_pDevice{ nullptr },
  m_pDeviceContext{ nullptr },
  m_pRenderTargetView{ nullptr },
  m_swapChainDesc{ 0 }
{}

/*
  Class destrcutor
*/
OpDirect3d11Base::~OpDirect3d11Base() {
  if (m_pDevice)            m_pDevice->Release();
  if (m_pDeviceContext)     m_pDeviceContext->Release();
  if (m_pSwapChain)         m_pSwapChain->Release();
  if (m_pRenderTargetView)  m_pRenderTargetView->Release();
}

/*
  @brief Initializes Direct3D11. create a Device and a swap chain.
  @param HWND _outputWindow - Window handle to the draw area.
  @return bool - Did D3D11 initialized successfully.
*/
bool OpDirect3d11Base::Initialize(HWND _outputWindow)
{
  // Empty Memory
  ZeroMemory(&m_swapChainDesc, sizeof(m_swapChainDesc));

  // Set Swap chain description
  m_swapChainDesc.BufferDesc.Width = 0;
  m_swapChainDesc.BufferDesc.Height = 0;
  m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
  m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
  m_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
  m_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

  m_swapChainDesc.SampleDesc.Count = 1;
  m_swapChainDesc.SampleDesc.Quality = 0;

  m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  m_swapChainDesc.BufferCount = 1;
  m_swapChainDesc.OutputWindow = _outputWindow;
  m_swapChainDesc.Windowed = TRUE;
  m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  m_swapChainDesc.Flags = 0;

  HRESULT hr{ S_OK };

  hr = D3D11CreateDeviceAndSwapChain(
    nullptr,
    D3D_DRIVER_TYPE_HARDWARE,
    nullptr,
    0,
    nullptr,
    0,
    D3D11_SDK_VERSION,
    &m_swapChainDesc,
    &m_pSwapChain,
    &m_pDevice,
    nullptr,
    &m_pDeviceContext
  );

  ID3D11Resource* pBackBuffer{ 0 };
  m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

  if (pBackBuffer == nullptr) return false;

  m_pDevice->CreateRenderTargetView(
    pBackBuffer,
    nullptr,
    &m_pRenderTargetView
  );

  pBackBuffer->Release();

  return SUCCEEDED(hr) == TRUE;
}

void OpDirect3d11Base::ClearBuffer(float red, float green, float blue) noexcept
{
  const float color[] = { red, green, blue, 1.0f };
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
}

void OpDirect3d11Base::ClearBuffer(const op::color::ColorHex fillColor) noexcept
{
  float color[4]{};
  op::color::SetHexArray(color, fillColor);
  m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
}


void OpDirect3d11Base::EndFrame()
{
  /*
    Present the rendered image to the user
  */
  m_pSwapChain->Present(1u, 0u);;
}

/*
void OpDirect3d11Base::CreateReferenceDevice(HWND _outputWindow)
{
  D3D_FEATURE_LEVEL levels[]{
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_1,
  };
  // Define inital parameters of swap chain
  // Request feature level implementing application's features needs.
  D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_11_0;
}
*/