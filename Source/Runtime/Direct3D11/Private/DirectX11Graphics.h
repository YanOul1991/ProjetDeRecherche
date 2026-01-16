/* ======================================================================================
 *  DirectX11Graphics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Types/Color.h"
#include "Core/Object/Image/Image.h"
#include "Runtime/Direct3D11/IDirect3D11.h"
#include "Private/Resources/IDirectX11Resource.h"
#include "Private/Resources/Buffer/DirectX11Buffer.h"
#include "Resources/Primitives.h"
#include <vector>

struct ConstColors
{
  struct {
    float r;
    float g;
    float b;
    float a;
  } face_colors[6];
};

struct Transform
{
  float x;
  float y;
  float z;
};

class MeshRenderer
{
public:
  MeshRenderer() = default;

  Transform position;
  Mesh* meshData;
};

class Sampler
{
public:
  inline Sampler() = default;
  inline ~Sampler()
  {}

  inline void init(ID3D11Device* pDevice)
  {
    D3D11_SAMPLER_DESC samplerDesc{};

    samplerDesc.Filter    = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU  = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV  = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW  = D3D11_TEXTURE_ADDRESS_WRAP;

    HRESULT hr{S_OK};
    OPTIM_TRY_DX(pDevice->CreateSamplerState(&samplerDesc, &pSampler));
  }

  inline void bind(ID3D11DeviceContext* pContext)
  {
    pContext->PSSetSamplers(0, 1, pSampler.GetAddressOf());
  }

  ComPtr<ID3D11SamplerState> pSampler;
};

class Texture
{
public:
  inline Texture() { }

  inline void allocResource(ID3D11Device* pDevice, Image* image) 
  {
    HRESULT                         hr{ S_OK };
    D3D11_TEXTURE2D_DESC            textDesc{};
    D3D11_SUBRESOURCE_DATA          subRes{};

    textDesc.Width              = image->width;
    textDesc.Height             = image->height;
    textDesc.MipLevels          = 1;
    textDesc.ArraySize          = 1;
    textDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    textDesc.SampleDesc.Count   = 1;
    textDesc.SampleDesc.Quality = 0;
    textDesc.Usage              = D3D11_USAGE_DEFAULT;
    textDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    textDesc.CPUAccessFlags     = 0;
    textDesc.MiscFlags          = 0;

    subRes.pSysMem          = image->pixels;
    subRes.SysMemPitch      = image->width * sizeof(op::color::SColor);
    subRes.SysMemSlicePitch = 0;

    OPTIM_TRY_DX(pDevice->CreateTexture2D(&textDesc, &subRes, &pResource));

    // Shader Resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

    srvDesc.Format                    = textDesc.Format;
    srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels       = 1;

    OPTIM_TRY_DX(pDevice->CreateShaderResourceView(pResource.Get(), &srvDesc, &pResourceView));

    printf("[DirectX11] texture resources allocated.\n");
  }

  inline void bind(ID3D11DeviceContext* pContext)
  {
    pContext->PSSetShaderResources(0, 1, pResourceView.GetAddressOf());
  }

  ComPtr<ID3D11Texture2D>           pResource     { nullptr };
  ComPtr<ID3D11ShaderResourceView>  pResourceView { nullptr };
};

struct DxColor 
{
  uint8 r;
  uint8 g;
  uint8 b;
  uint8 a;
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
  DirectX::XMMATRIX matrix_projection{};
  DirectX::XMMATRIX matrix_camera{};

  Mesh _cubeMesh;

  std::vector<MeshRenderer> objects;

  ConstantBuffer<DirectX::XMMATRIX>   __t_constBuffer{};
  ConstantBuffer<ConstColors>         __t_constBufferColor{};

  GFXMaterial __t_material{};

  Texture _test_texture{};
  Sampler _test_sampler{};
};