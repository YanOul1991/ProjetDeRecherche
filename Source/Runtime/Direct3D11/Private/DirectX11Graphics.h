/* ======================================================================================
 *  DirectX11Graphics.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Runtime/Direct3D11/IDirect3D11.h"
#include "Core/Color/Color.h"

class   DirectX11Buffer;
class   VertexBuffer;
class   IndexBuffer;

struct SGFXVertex
{
  struct
  {
    float x;
    float y;
    float z;
  } position;
};

class DirectX11Buffer
{
public:
  virtual ~DirectX11Buffer() = default;
  ComPtr<ID3D11Buffer>    m_comptr;
  D3D11_BUFFER_DESC       m_bufferDesc{};
  D3D11_SUBRESOURCE_DATA  m_bufferSubres{};

  int32   m_bufferByteSize  { 0 };
  int32   m_elementCount    { 0 };
  uint32  m_stride          { 0 };
  uint32  m_offset          { 0 };
};

class VertexBuffer : public DirectX11Buffer
{
public:
  inline VertexBuffer() = default;

  inline VertexBuffer(SGFXVertex vertices[], int32 bufferByteSize)
  {
    m_bufferByteSize = bufferByteSize;
    m_elementCount = bufferByteSize / sizeof(SGFXVertex);
    m_stride = sizeof(SGFXVertex);
    m_offset = 0;

    m_bufferDesc.ByteWidth            = bufferByteSize;
    m_bufferDesc.StructureByteStride  = sizeof(SGFXVertex);
    m_bufferDesc.Usage                = D3D11_USAGE_DEFAULT;
    m_bufferDesc.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
    m_bufferDesc.CPUAccessFlags       = 0;
    m_bufferDesc.MiscFlags            = 0;

    m_bufferSubres.pSysMem            = vertices;
  }
};

class IndexBuffer : public DirectX11Buffer
{
public:
  inline IndexBuffer() = default;

  inline IndexBuffer(uint16 indices[], int32 byteSize)
  {
    m_bufferByteSize = byteSize;
    m_elementCount = byteSize / sizeof(uint16);
    m_stride = 0;
    m_offset = 0;

    m_bufferDesc.ByteWidth            = byteSize;
    m_bufferDesc.StructureByteStride  = sizeof(uint16);
    m_bufferDesc.Usage                = D3D11_USAGE_DEFAULT;
    m_bufferDesc.BindFlags            = D3D11_BIND_INDEX_BUFFER;
    m_bufferDesc.CPUAccessFlags       = 0;
    m_bufferDesc.MiscFlags            = 0;
    m_bufferSubres.pSysMem            = indices;
  }
};

template <typename T>
class ConstantBuffer : public DirectX11Buffer
{
public:
  inline ConstantBuffer() = default;

  inline ConstantBuffer(T&& initial_data)
  {
    data = initial_data;

    m_bufferByteSize  = sizeof(T);
    m_elementCount    = m_bufferByteSize / sizeof(T);
    m_stride          = 0;
    m_offset          = 0;

    m_bufferDesc.ByteWidth            = sizeof(T);
    m_bufferDesc.StructureByteStride  = 0;
    m_bufferDesc.Usage                = D3D11_USAGE_DEFAULT;
    m_bufferDesc.BindFlags            = D3D11_BIND_CONSTANT_BUFFER;
    m_bufferDesc.CPUAccessFlags       = 0;
    m_bufferDesc.MiscFlags            = 0;

    m_bufferSubres.pSysMem            = &data;
  }

  T data{};
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

  DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

  bool initialize(HWND _outputWindow);
  void presentBuffer();
  void clearBuffer(float red, float green, float blue, float alpha);
  void drawTriangle(float __offset, float __angle, float __posX, float __posY);

  void renderUpdate();
  void createBuffer(DirectX11Buffer& buffer);

private:
  ComPtr<ID3D11Device>            m_pDevice;
  ComPtr<IDXGISwapChain>          m_pSwapChain;
  ComPtr<ID3D11DeviceContext>     m_pDeviceContext;
  ComPtr<ID3D11RenderTargetView>  m_pRenderTargetView;
  ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;

  /// TEST FIELDS
  VertexBuffer  __t_VertexBuffer;
  IndexBuffer   __t_IndexBuffer;

  ConstantBuffer<DirectX::XMMATRIX>   __t_constBuffer{};
  ConstantBuffer<ConstColors>         __t_constBufferColor{};

  SGFXVertex* __t_vertexData;
  uint16*     __t_indexData;
};