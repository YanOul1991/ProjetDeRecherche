/* ======================================================================================
 *  DirectX11Graphics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Runtime/Direct3D11/IDirect3D11.h"
#include "Core/Color/Color.h"

struct DirectX11Buffer
{
  ComPtr<ID3D11Buffer>    comptr;
  D3D11_BUFFER_DESC       desc;
  D3D11_SUBRESOURCE_DATA  subresource;
};

class DirectX11Graphics final
{
public:
  DirectX11Graphics();
  ~DirectX11Graphics();

  // No move or copy
  DirectX11Graphics(const DirectX11Graphics&)             = delete;
  DirectX11Graphics(const DirectX11Graphics&&)            = delete;
  DirectX11Graphics& operator=(const DirectX11Graphics&)  = delete;
  DirectX11Graphics& operator=(const DirectX11Graphics&&) = delete;

  DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

  bool initialize(HWND _outputWindow);
  void presentBuffer();
  void clearBuffer(float red, float green, float blue);
  void clearBuffer(const op::color::ColorRgb fillColor);
  void drawTriangle(float __offset, float __angle, float __posX, float __posY);

private:
  ComPtr<ID3D11Device>            m_pDevice;
  ComPtr<IDXGISwapChain>          m_pSwapChain;
  ComPtr<ID3D11DeviceContext>     m_pDeviceContext;
  ComPtr<ID3D11RenderTargetView>  m_pRenderTargetView;

};