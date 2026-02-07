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

/*
class IDirectX11Buffer
{
public:
  virtual ~IDirectX11Buffer() {}

  virtual void init(ID3D11Device* pDevice)         = 0;
  virtual void bindResource(ID3D11DeviceContext* pContext) = 0;

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

  inline void bindResource(ID3D11DeviceContext* pContext) override { 
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
*/

class Dx11ConstantBuffer
{
public:
  inline Dx11ConstantBuffer(ID3D11Device* pDevice, uint32 param_structByteSize)
  {
    byteSize = param_structByteSize;
    D3D11_BUFFER_DESC desc{};

    desc.ByteWidth        = byteSize;
    desc.Usage            = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    desc.BindFlags        = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags   = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags        = 0;

    pDevice->CreateBuffer(&desc, nullptr, &pBuffer);
  }

  inline void bindResource(ID3D11DeviceContext* pContext)
  {
    pContext->VSSetConstantBuffers(0, 1, pBuffer.GetAddressOf());
  }

  inline void update(ID3D11DeviceContext* pContext, void* param_pNewData) const
  {
    D3D11_MAPPED_SUBRESOURCE mapped{};
    pContext->Map(pBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, param_pNewData, byteSize);
    pContext->Unmap(pBuffer.Get(), 0);
  }

  ComPtr<ID3D11Buffer> pBuffer{};
  uint32 byteSize{};
};


// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++

// IF THE BACKBUFFER NEEDS TO BE REIZED DUE TO
// WINDOW RESIZE. THE INSTANCE MUST BE DELETED
// AND RECREATED .

class Dx11DepthStencil final
{
public:
  inline Dx11DepthStencil(ID3D11Device* pDevice, SDepthStencilDescription* param_pDepthStencilDesc) {
    OPTIM_CHECK_WIN_COM();

    D3D11_DEPTH_STENCIL_DESC l_dsDesc{};
    l_dsDesc.DepthEnable    = param_pDepthStencilDesc->depthTestEnabled;
    l_dsDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(static_cast<int32>(param_pDepthStencilDesc->depthWriteMask));
    l_dsDesc.DepthFunc      = static_cast<D3D11_COMPARISON_FUNC>(static_cast<int32>(param_pDepthStencilDesc->depthComparisonFunction) + 1);
    l_dsDesc.StencilEnable  = FALSE;

    OPTIM_TRY_DX(pDevice->CreateDepthStencilState(&l_dsDesc, &pState));
  }

  inline void bindResource(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) const {
    pContext->OMSetDepthStencilState(pState.Get(), 1);
  }

  ComPtr<ID3D11DepthStencilState> pState{};
};

// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++


// IF THE BACKBUFFER NEEDS TO BE REIZED DUE TO
// WINDOW RESIZE. THE INSTANCE MUST BE DELETED
// AND RECREATED .

class Dx11DepthStencilViewTexture
{
public:

  inline Dx11DepthStencilViewTexture(ID3D11Device* pDevice)
  {
    OPTIM_CHECK_WIN_COM();

    D3D11_TEXTURE2D_DESC depthDesc{};

    depthDesc.Width               = 1920;
    depthDesc.Height              = 1080;
    depthDesc.MipLevels           = 1;
    depthDesc.ArraySize           = 1;
    depthDesc.Format              = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.Usage               = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags           = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.SampleDesc.Count    = 1;
    depthDesc.SampleDesc.Quality  = 0;

    OPTIM_TRY_DX(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

    // Depth stencil view creation
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

    dsvDesc.Format              = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension       = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice  = 0;

    OPTIM_TRY_DX(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDepthStencilView));
  }

  inline Dx11DepthStencilViewTexture(ID3D11Device* pDevice, uint32 param_width, uint32 param_height)
  {
    OPTIM_CHECK_WIN_COM();

    D3D11_TEXTURE2D_DESC depthDesc{};

    depthDesc.Width               = param_width;
    depthDesc.Height              = param_height;
    depthDesc.MipLevels           = 1;
    depthDesc.ArraySize           = 1;
    depthDesc.Format              = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.Usage               = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags           = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    depthDesc.SampleDesc.Count    = 1;
    depthDesc.SampleDesc.Quality  = 0;

    OPTIM_TRY_DX(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

    // Depth stencil view creation
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

    dsvDesc.Format              = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension       = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice  = 0;

    OPTIM_TRY_DX(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDepthStencilView));
  }

  inline void resize(ID3D11Device* pDevice, uint32 newWidth, uint32 newHeight) {
    OPTIM_CHECK_WIN_COM();

    D3D11_TEXTURE2D_DESC l_depthDesc  {};
    D3D11_DEPTH_STENCIL_VIEW_DESC l_dsvDesc{};

    pDepthStencil->GetDesc(&l_depthDesc);
    pDepthStencilView->GetDesc(&l_dsvDesc);

    l_depthDesc.Width   = newWidth;
    l_depthDesc.Height  = newHeight;

    pDepthStencil.Reset();
    pDepthStencilView.Reset();

    OPTIM_WIN_THROW_ON_FAILED(pDevice->CreateTexture2D(&l_depthDesc, nullptr, &pDepthStencil));
    OPTIM_WIN_THROW_ON_FAILED(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &l_dsvDesc, &pDepthStencilView));
  }

  inline void bindResource(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) const {
    pContext->OMSetRenderTargets(1, ppRenderTargetView, pDepthStencilView.Get());
  }

  inline void clearDepthStencilView(ID3D11DeviceContext* pContext) const {
    pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  ComPtr<ID3D11Texture2D>         pDepthStencil{};
  ComPtr<ID3D11DepthStencilView>  pDepthStencilView{};
};


// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++

class Dx11RasterizerState final
{
public:
  inline Dx11RasterizerState(SRasterizerDescription* param_pRasterizerDesc) {
    D3D11_RASTERIZER_DESC rastDesc{};
    rastDesc.FillMode               = static_cast<D3D11_FILL_MODE>(static_cast<int32>(param_pRasterizerDesc->fillMode) + 2);
    rastDesc.CullMode               = static_cast<D3D11_CULL_MODE>(static_cast<int32>(param_pRasterizerDesc->cullMode) + 1);
    rastDesc.FrontCounterClockwise  = param_pRasterizerDesc->faceWinding == ERasterizerFaceWinding::CounterClockWise;
    rastDesc.DepthBias              = param_pRasterizerDesc->depthBias;
    rastDesc.DepthBiasClamp         = 0.0f;
    rastDesc.SlopeScaledDepthBias   = param_pRasterizerDesc->slopeScaledDepthBias;
    rastDesc.DepthClipEnable        = TRUE;
    rastDesc.ScissorEnable          = FALSE;
    rastDesc.MultisampleEnable      = FALSE;
    rastDesc.AntialiasedLineEnable  = TRUE;

    Dx11RHI::getDevicePtr()->CreateRasterizerState(&rastDesc, &pRasterizer);
  }

  inline void bindResource() const
  {
    Dx11RHI::getContextPtr()->RSSetState(pRasterizer.Get());
  }

  ComPtr<ID3D11RasterizerState> pRasterizer{};
};