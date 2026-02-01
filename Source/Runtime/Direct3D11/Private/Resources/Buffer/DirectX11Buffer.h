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
  inline Dx11DepthStencil(ID3D11Device* pDevice, SDepthStencilDescription* param_pDepthStencilDesc)
  {
    HRESULT hr{ S_OK };

    /* ////////////////////////////////////
     * DEPTH STENCIL STATE CREATION
    //////////////////////////////////// */
    D3D11_DEPTH_STENCIL_DESC l_dsDesc{};

    l_dsDesc.DepthEnable = static_cast<bool>(param_pDepthStencilDesc->depthTestEnabled);

    if (param_pDepthStencilDesc->depthWriteMask == EDepthStencilDepthWriteMask::WriteAll) {
      l_dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    }
    else {
      l_dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    }

    l_dsDesc.DepthFunc = (D3D11_COMPARISON_FUNC)(((int32)param_pDepthStencilDesc->depthComparisonFunction) + 1);

    /*
    switch (param_pDepthStencilDesc->depthComparaisonFunction) {
      case EDepthStencilComparaisonFunction::Never: {
        l_dsDesc.DepthFunc = D3D11_COMPARISON_NEVER;
        break;
      }
      case EDepthStencilComparaisonFunction::Less: {
        l_dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
        break;
      }
      case EDepthStencilComparaisonFunction::LessOrEqual: {
        l_dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        break;
      }
      case EDepthStencilComparaisonFunction::Equal: {
        l_dsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;
        break;
      }
      case EDepthStencilComparaisonFunction::Greater: {
        l_dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
        break;
      }
      case EDepthStencilComparaisonFunction::GreaterOrEqual: {
        l_dsDesc.DepthFunc = D3D11_COMP
      }
      default: {
        break;
      }
    } // END OF SWITCH CASE
    */

    //l_dsDesc.DepthFunc      = D3D11_COMPARISON_LESS;
    l_dsDesc.StencilEnable  = FALSE;

    OPTIM_TRY_DX(pDevice->CreateDepthStencilState(&l_dsDesc, &pState));

    /* ////////////////////////////////////
     * DEPTH TEXTURE 2D CREATION
    //////////////////////////////////// */
    D3D11_TEXTURE2D_DESC depthDesc{};
    depthDesc.Width     = 1920;
    depthDesc.Height    = 1080;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format    = DXGI_FORMAT_D32_FLOAT;
    depthDesc.Usage     = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    depthDesc.SampleDesc.Count    = 1;
    depthDesc.SampleDesc.Quality  = 0;

    OPTIM_TRY_DX(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

    // Depth stencil view creation
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format              = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension       = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice  = 0;

    OPTIM_TRY_DX(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pView));
  }

  inline void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView)
  {
    pContext->OMSetDepthStencilState(pState.Get(), 1);
    pContext->OMSetRenderTargets(1, ppRenderTargetView, pView.Get());
  }

  inline void clearDepthStencilView(ID3D11DeviceContext* pContext) const
  {
    pContext->ClearDepthStencilView(pView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  ComPtr<ID3D11DepthStencilState> pState{};
  ComPtr<ID3D11Texture2D>         pDepthStencil{};
  ComPtr<ID3D11DepthStencilView>  pView{};
};

class Dx11DepthStencilViewTexture
{
public:
  inline Dx11DepthStencilViewTexture(ID3D11Device* pDevice)
  {
    HRESULT hr{S_OK};

    /* ////////////////////////////////////
     * DEPTH TEXTURE 2D CREATION
    //////////////////////////////////// */
    D3D11_TEXTURE2D_DESC depthDesc{};
    depthDesc.Width     = 1920;
    depthDesc.Height    = 1080;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format    = DXGI_FORMAT_D32_FLOAT;
    depthDesc.Usage     = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    depthDesc.SampleDesc.Count    = 1;
    depthDesc.SampleDesc.Quality  = 0;

    OPTIM_TRY_DX(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

    // Depth stencil view creation
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    dsvDesc.Format              = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension       = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice  = 0;

    OPTIM_TRY_DX(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pView));
  }

  void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView)
  {
    pContext->OMSetRenderTargets(1, ppRenderTargetView, pView.Get());
  }

  void clearDepthStencilView(ID3D11DeviceContext* pContext) const
  {
    pContext->ClearDepthStencilView(pView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  ComPtr<ID3D11Texture2D>         pDepthStencil{};
  ComPtr<ID3D11DepthStencilView>  pView{};
};


// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++

class Dx11RasterizerState final
{
public:
  inline Dx11RasterizerState(SRasterizerDescription* param_pRasterizerDesc)
  {
    D3D11_RASTERIZER_DESC rastDesc{};
    rastDesc.FillMode   = static_cast<D3D11_FILL_MODE>(static_cast<int32>(param_pRasterizerDesc->fillMode) + 2);
    rastDesc.CullMode   = static_cast<D3D11_CULL_MODE>(static_cast<int32>(param_pRasterizerDesc->cullMode) + 1);
    rastDesc.FrontCounterClockwise = param_pRasterizerDesc->faceWinding == ERasterizerFaceWinding::CounterClockWise;

    rastDesc.DepthBias              = 0;
    rastDesc.DepthBiasClamp         = 0.0f;
    rastDesc.SlopeScaledDepthBias   = 0;
    rastDesc.DepthClipEnable        = TRUE;
    rastDesc.ScissorEnable          = FALSE;
    rastDesc.MultisampleEnable      = FALSE;
    rastDesc.AntialiasedLineEnable  = FALSE;

    Dx11RHI::getDevicePtr()->CreateRasterizerState(&rastDesc, &pRasterizer);
  }

  void bind()
  {
    Dx11RHI::getContextPtr()->RSSetState(pRasterizer.Get());
  }

  ComPtr<ID3D11RasterizerState> pRasterizer{};
};