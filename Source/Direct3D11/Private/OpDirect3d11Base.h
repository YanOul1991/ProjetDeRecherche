#pragma once

#include "Global/OptimEngineGlobal.h"
#include "OpDirect3d11Global.h"

class OpDirect3d11Base final
{
public:
  OpDirect3d11Base();
  ~OpDirect3d11Base();

  // No move or copy
  OpDirect3d11Base(const OpDirect3d11Base&) = delete;
  OpDirect3d11Base(const OpDirect3d11Base&&) = delete;
  OpDirect3d11Base& operator=(const OpDirect3d11Base&) = delete;
  OpDirect3d11Base& operator=(const OpDirect3d11Base&&) = delete;

  //void CreateReferenceDevice(HWND _outputWindow);
  DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

  /*======================== FUNCTIONS ========================*/

  bool Initialize(HWND _outputWindow);    // Initialize Direct3d11.

  /*
    Present the rendered pixels to the screen.
  */
  void EndFrame();

  /*
    Clear the Render buffer.
  */

  void ClearBuffer(float red, float green, float blue) noexcept 
  {
    const float color[] = { red, green, blue, 1.0f };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
  }

  void ClearBuffer(const op::color::ColorHex fillColor) noexcept
  {
    float color[4]{};
    op::color::SetHexArray(color, fillColor);
     m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
  }

private:
  ID3D11Device* m_pDevice;
  IDXGISwapChain* m_pSwapChain;
  ID3D11DeviceContext* m_pDeviceContext;
  ID3D11RenderTargetView* m_pRenderTargetView; 

  /*
  ID3D11Texture2D* m_pDepthStencilBuffer;
  ID3D11DepthStencilState* m_pDepthStencilState;
  ID3D11DepthStencilView* m_pDepthStencilView;
  ID3D11RasterizerState* m_pRasterizeState;

  DirectX::XMMATRIX m_worldMatrix;
  DirectX::XMMATRIX m_orthMatrix;
  D3D11_VIEWPORT m_viewport;
  */
};