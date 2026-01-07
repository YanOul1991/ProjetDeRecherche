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

/* --------------------------------------
    VERTEX BUFFER
-------------------------------------- */

class DirectX11Buffer
{
public:
  virtual ~DirectX11Buffer() = default;

  virtual void create(ID3D11Device* device)       = 0;
  virtual void bind(ID3D11DeviceContext* context) = 0;

  ComPtr<ID3D11Buffer>    m_comptr;
  int32   m_bufferByteSize  { 0 };
  int32   m_elementCount    { 0 };
  uint32  m_stride          { 0 };
  uint32  m_offset          { 0 };
};

/* --------------------------------------
    VERTEX BUFFER
-------------------------------------- */
template <typename T> 
class VertexBuffer : public DirectX11Buffer
{
public:
  inline VertexBuffer() = default;

  inline VertexBuffer(T* vertices, int32 bufferByteSize)
  {
    m_bufferByteSize  = bufferByteSize;
    m_elementCount    = bufferByteSize / sizeof(T);
    m_stride          = sizeof(T);
    m_offset          = 0;
    data              = vertices;
  }

  inline void create(ID3D11Device* device) override
  { 
    D3D11_BUFFER_DESC       desc{};
    D3D11_SUBRESOURCE_DATA  subres{};

    desc.ByteWidth            = m_bufferByteSize;
    desc.StructureByteStride  = sizeof(T);
    desc.Usage                = D3D11_USAGE_DEFAULT;
    desc.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags       = 0;
    desc.MiscFlags            = 0;
    subres.pSysMem            = data;

    HRESULT hr{S_OK};
    OPTIM_TRY_DX(device->CreateBuffer(&desc, &subres, &m_comptr));
  }

  inline void bind(ID3D11DeviceContext* context) override 
  { 
    context->IASetVertexBuffers(0, 1, m_comptr.GetAddressOf(), &m_stride, &m_offset);
  }

  T* data{ nullptr };
};

/* --------------------------------------
    INDEX BUFFER
-------------------------------------- */

class IndexBuffer : public DirectX11Buffer
{
public:
  inline IndexBuffer() = default;

  inline IndexBuffer(uint16* indices, int32 byteSize)
  {
    m_bufferByteSize  = byteSize;
    m_elementCount    = byteSize / sizeof(uint16);
    m_stride          = 0;
    m_offset          = 0;
    data              = indices;
  }

  inline void create(ID3D11Device* device) override 
  {
    D3D11_BUFFER_DESC       desc{};
    D3D11_SUBRESOURCE_DATA  subres{};

    desc.ByteWidth            = m_bufferByteSize;
    desc.StructureByteStride  = sizeof(uint16);
    desc.Usage                = D3D11_USAGE_DEFAULT;
    desc.BindFlags            = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags       = 0;
    desc.MiscFlags            = 0;
    subres.pSysMem            = data;

    HRESULT hr{ S_OK };
    OPTIM_TRY_DX(device->CreateBuffer(&desc, &subres, &m_comptr));
  }

  inline void bind(ID3D11DeviceContext* context) override 
  { 
    context->IASetIndexBuffer(m_comptr.Get(), DXGI_FORMAT_R16_UINT, 0);
  }

  uint16* data{ nullptr };
};

/* -------------------------------------- 
    CONSTANT BUFFER
-------------------------------------- */

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
    data              = initial_data;
  }

  inline void create(ID3D11Device* device) override 
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

    device->CreateBuffer(&desc, &subres, &m_comptr);
  }

  inline void bind(ID3D11DeviceContext* context) override { }

  inline void update(ID3D11DeviceContext* context)
  {
    D3D11_MAPPED_SUBRESOURCE mapped;
    context->Map(m_comptr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &data, sizeof(T));
    context->Unmap(m_comptr.Get(), 0);
  }

  T data{};
};