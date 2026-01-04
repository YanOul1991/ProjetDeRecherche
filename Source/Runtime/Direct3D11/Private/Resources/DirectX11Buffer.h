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

class DirectX11Buffer
{
public:
  virtual ~DirectX11Buffer() = default;
  ComPtr<ID3D11Buffer>    m_comptr;
  D3D11_BUFFER_DESC       m_bufferDesc{};
  D3D11_SUBRESOURCE_DATA  m_bufferSubres{};

  int32   m_bufferByteSize{ 0 };
  int32   m_elementCount{ 0 };
  uint32  m_stride{ 0 };
  uint32  m_offset{ 0 };
};

template <typename T>
class VertexBuffer : public DirectX11Buffer
{
public:
  inline VertexBuffer() = default;

  inline VertexBuffer(T vertices[], int32 bufferByteSize)
  {
    m_bufferByteSize  = bufferByteSize;
    m_elementCount    = bufferByteSize / sizeof(T);

    m_stride = sizeof(T);
    m_offset = 0;

    m_bufferDesc.ByteWidth            = bufferByteSize;
    m_bufferDesc.StructureByteStride  = sizeof(T);
    m_bufferDesc.Usage                = D3D11_USAGE_DEFAULT;
    m_bufferDesc.BindFlags            = D3D11_BIND_VERTEX_BUFFER;
    m_bufferDesc.CPUAccessFlags       = 0;
    m_bufferDesc.MiscFlags            = 0;

    m_bufferSubres.pSysMem = vertices;
  }
};

class IndexBuffer : public DirectX11Buffer
{
public:
  inline IndexBuffer() = default;

  inline IndexBuffer(uint16 indices[], int32 byteSize)
  {
    m_bufferByteSize  = byteSize;
    m_elementCount    = byteSize / sizeof(uint16);

    m_stride = 0;
    m_offset = 0;

    m_bufferDesc.ByteWidth              = byteSize;
    m_bufferDesc.StructureByteStride    = sizeof(uint16);
    m_bufferDesc.Usage                  = D3D11_USAGE_DEFAULT;
    m_bufferDesc.BindFlags              = D3D11_BIND_INDEX_BUFFER;
    m_bufferDesc.CPUAccessFlags         = 0;
    m_bufferDesc.MiscFlags              = 0;

    m_bufferSubres.pSysMem              = indices;
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