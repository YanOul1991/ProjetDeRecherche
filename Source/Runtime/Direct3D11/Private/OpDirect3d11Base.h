/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      ~~~~~~~~~~ MAY BE REMOVED/REFORMATTED ~~~~~~~~~~
      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      
      + OpDirect3d11.h:
          Abstracting class for OpDirect3D11Module.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */


#pragma once

#include "OpDirect3d11Global.h"
#include "Core/Color/Color.h"

//namespace op::

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

  DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

  bool Initialize(HWND _outputWindow);    // Initialize Direct3d11.
  void EndFrame();
  void clearBuffer(float red, float green, float blue) noexcept;
  void clearBuffer(const op::color::ColorHex fillColor) noexcept;
  void clearBuffer(const op::color::ColorRgb fillColor) noexcept;

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