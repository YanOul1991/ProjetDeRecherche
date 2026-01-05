/* ======================================================================================
 *  DirectX11Graphics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Runtime/Direct3D11/IDirect3D11.h"
#include "Core/Color/Color.h"

#include "Resources/DirectX11Resources.h"

struct SGFXVertex
{
  struct
  {
    float x;
    float y;
    float z;
  } position;
};

struct ConstColors
{
  struct {
    float r;
    float g;
    float b;
    float a;
  } face_colors[6];
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

  bool initialize(HWND _outputWindow);
  void presentBuffer();
  void clearBuffer(float red, float green, float blue, float alpha);
  void renderUpdate();

private:
  ComPtr<ID3D11Device>            m_pDevice;
  ComPtr<IDXGISwapChain>          m_pSwapChain;
  ComPtr<ID3D11DeviceContext>     m_pContext;
  ComPtr<ID3D11RenderTargetView>  m_pRenderTargetView;
  ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;

  /// TEST FIELDS
  VertexBuffer<SGFXVertex>  __t_VertexBuffer{};
  IndexBuffer               __t_IndexBuffer{};

  ConstantBuffer<DirectX::XMMATRIX>   __t_constBuffer{};
  ConstantBuffer<ConstColors>         __t_constBufferColor{};

  GFXMaterial __t_material{};

  SGFXVertex*   __t_vertexData  { nullptr };
  uint16*      __t_indexData    { nullptr };
};