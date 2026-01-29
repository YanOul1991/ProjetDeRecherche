/* ======================================================================================
 *  DirectX11Buffer.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Runtime/Direct3D11/Dx11RHI.h"

class IDirectX11Buffer
{
public:
  virtual ~IDirectX11Buffer() {}

  virtual void init(ID3D11Device* pDevice)         = 0;
  virtual void bind(ID3D11DeviceContext* pContext) = 0;

  ComPtr<ID3D11Buffer> pBuffer;
  int32   bufferByteSize  { 0 };
  int32   elementCount    { 0 };
  uint32  stride          { 0 };
  uint32  offset          { 0 };

};

template <typename T> 
class ConstantBuffer final : public IDirectX11Buffer
{
public:
  inline ConstantBuffer() {
  }
  inline ~ConstantBuffer() override
  {}

  inline ConstantBuffer(T&& initial_data)
  {
    data            = initial_data;
    bufferByteSize  = sizeof(T);
    elementCount    = bufferByteSize / sizeof(T);
    stride          = 0;
    offset          = 0;
    data            = initial_data;
  }

  inline void init(ID3D11Device* pDevice) override 
  { 
    D3D11_BUFFER_DESC       desc{};
    D3D11_SUBRESOURCE_DATA  subres{};

    desc.ByteWidth            = sizeof(T);
    desc.StructureByteStride  = 0;
    desc.Usage                = D3D11_USAGE_DYNAMIC;
    desc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags       = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags            = 0;

    subres.pSysMem            = &data;

    pDevice->CreateBuffer(&desc, &subres, &pBuffer);
    //printf("Element at 0x%p is a Constant buffer resource\n", this);
  }

  inline void bind(ID3D11DeviceContext* pContext) override { 
    pContext->VSSetConstantBuffers(0, 1, pBuffer.GetAddressOf());
  }

  inline void update(ID3D11DeviceContext* pContext) {
    D3D11_MAPPED_SUBRESOURCE mapped;
    pContext->Map(pBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &data, sizeof(T));
    pContext->Unmap(pBuffer.Get(), 0);
  }

  T data{};
};

// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++

class Dx11DepthStencil final
{
public:
  inline Dx11DepthStencil()
  {
    // Depth stencil description
    D3D11_DEPTH_STENCIL_DESC dsDesc{};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Dx11RHI::getDevicePtr()->CreateDepthStencilState(&dsDesc, &pState);

    // Depth texture creation
    D3D11_TEXTURE2D_DESC texDesc{};
    texDesc.Width     = 1920;
    texDesc.Height    = 1080;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format    = DXGI_FORMAT_R32_FLOAT;
    texDesc.Usage     = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    texDesc.SampleDesc.Count    = 1;
    texDesc.SampleDesc.Quality  = 0;
    Dx11RHI::getDevicePtr()->CreateTexture2D(&texDesc, 0, &pDepthStencilTexture);

    // Depth stencil view creation
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format              = DXGI_FORMAT_R32_FLOAT;
    dsvDesc.ViewDimension       = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice  = 0;

    Dx11RHI::getDevicePtr()->CreateDepthStencilView(pDepthStencilTexture.Get(), &dsvDesc, &pView);
  }

  void bind(ID3D11RenderTargetView** ppRenderTargetView)
  {
    // Bind depth stencil state
    Dx11RHI::getContextPtr()->OMSetDepthStencilState(pState.Get(), 1);
    // bind depth stencil view
    Dx11RHI::getContextPtr()->OMSetRenderTargets(1, ppRenderTargetView, pView.Get());
  }

  ComPtr<ID3D11Texture2D>         pDepthStencilTexture{};
  ComPtr<ID3D11DepthStencilState> pState{};
  ComPtr<ID3D11DepthStencilView>  pView{};
};


// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++

class Dx11RasterizerState final
{
public:
  inline Dx11RasterizerState()
  {
    D3D11_RASTERIZER_DESC rastDesc{};

    rastDesc.FillMode               = D3D11_FILL_SOLID;
    rastDesc.CullMode               = D3D11_CULL_BACK;
    rastDesc.FrontCounterClockwise  = TRUE;

    Dx11RHI::getDevicePtr()->CreateRasterizerState(&rastDesc, &pRasterizer);
  }

  void bind()
  {
    Dx11RHI::getContextPtr()->RSSetState(pRasterizer.Get());
  }

  ComPtr<ID3D11RasterizerState> pRasterizer{};
};