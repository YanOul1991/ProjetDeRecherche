/* ======================================================================================
 *  IDirect3D11.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/System/Application.h"
#include "Core/Time/Time.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/Input/Input.h"

#include "DirectX11Graphics.h"
#include "Runtime/Direct3D11/IDirect3D11.h"

#include "Private/Resources/Dx11VertexBuffer.h"
#include "Private/Resources/Dx11IndexBuffer.h"
#include "Private/Resources/Dx11VertexShader.h"
#include "Private/Resources/Dx11PixelShader.h"
#include "Private/Resources/Dx11Texture.h"
#include "Private/Resources/Dx11Sampler.h"

#include <cmath>
#include <vector>



extern "C" DIRECTX11_API IDirect3D11* CreateDirect3D11Module() {
  return new IDirect3D11;
}

IDirect3D11::IDirect3D11() :
  m_hTargetWindow{ nullptr },
  m_pBase{ nullptr }
{}

IDirect3D11::~IDirect3D11() {}

void IDirect3D11::Initialize(void* _WindowHandle)
{
  m_hTargetWindow =_WindowHandle;
  m_pBase = new DirectX11Graphics{};
  if (m_pBase) { 
    m_pBase->initialize(reinterpret_cast<HWND>(m_hTargetWindow));
  }
}

// ##################################
//    DRAW CALL
// ##################################

void IDirect3D11::draw() {
  m_pBase->clearBuffer((34.0f / 255.0f), (38.0f / 255.0f), (92.0f / 255.0f), 1.0f);
  m_pBase->renderUpdate();
  m_pBase->presentBuffer();
}

void IDirect3D11::Clean() {}

ID3D11Device* IDirect3D11::getDevicePtr() {
  return DirectX11Graphics::deviceRef;
}

ID3D11DeviceContext* IDirect3D11::getContextPtr() {
  return DirectX11Graphics::contextRef;
}

// ##################################
//    RESOURCE CREATION FUNCTIONS
// ##################################

IVertexBuffer* IDirect3D11::createVertexBuffer(Vertex* pVertices, const uint32& bufferElementCount) {
  Dx11VertexBuffer* pResource = new Dx11VertexBuffer;
  pResource->createResources(pVertices, bufferElementCount);
  return pResource;
}

IIndexBuffer* IDirect3D11::createIndexBuffer(uint32* pIndices, const uint32& bufferElementCount) {
  Dx11IndexBuffer* pResource = new Dx11IndexBuffer;
  pResource->createResources(pIndices, bufferElementCount);
  return pResource;
}

IVertexShader* IDirect3D11::createVertexShader(const wchar* path) {
  Dx11VertexShader* pResource = new Dx11VertexShader;
  pResource->createResources(path);
  return pResource;
}

IPixelShader* IDirect3D11::createPixelShader(const wchar* path) {
  Dx11PixelShader* pResource = new Dx11PixelShader;
  pResource->createResources(path);
  return pResource;
}

ITextureResource* IDirect3D11::createTextureResource(const Image* pImage) {
  Dx11TextureResource* pResource = new Dx11TextureResource;
  pResource->createResource(pImage);
  return pResource;
}

ISampler* IDirect3D11::createSamplerResource() {
  Dx11Sampler* pResource = new Dx11Sampler;
  pResource->createResource();
  return pResource;
}

// ##################################
//    BINDING FUNCTIONS
// ##################################

void IDirect3D11::bindVertexBuffer(IVertexBuffer* pVertexBuffer) {
  pVertexBuffer->bindResource();
}

void IDirect3D11::bindIndexBuffer(IIndexBuffer* pIndexBuffer) {
  pIndexBuffer->bindResource();
}

void IDirect3D11::bindVertexShader(IVertexShader* pVertexShader) {
  pVertexShader->bindResource();
}

void IDirect3D11::bindPixelShader(IPixelShader* pPixelShader) {
  pPixelShader->bindResource();
}

void IDirect3D11::bindTexture(ITextureResource* pTexture) {
  pTexture->bindResource();
}

void IDirect3D11::bindSampler(ISampler* pSampler) {
  pSampler->bindResource();
}

template<typename T>
struct GraphicResourceEntry {
  T* pResource{nullptr};
  uint32 gen{0};
  bool valid{true};
};

using Dx11PixelShaderEntry     = GraphicResourceEntry<Dx11PixelShader>;
using Dx11VertexShaderEntry    = GraphicResourceEntry<Dx11VertexShader>;
using Dx11TextureResourceEntry = GraphicResourceEntry<Dx11TextureResource>;

/*
template<typename T>
class ResourcePool 
{
public:
  struct Entry {
    T* resource;
    bool valid;
  };

  std::vector<Entry>  entries{};
  std::vector<uint32> free{};
};
*/

template<typename T>
class GFXResourcePoll
{
public:
  SGraphicResourceHandle add(T entry) {
    SGraphicResourceHandle handle{};
    uint32 index{};
    uint32 generation{};

    if (freeIndices.size() == 0) {
      index = (uint32)(entries.size());
      generation = 1;
      entries.push_back(entry);
    }
    else {
      index = freeIndices[0];
      generation = entries[index].gen;

      entry.valid = true;
      entry.gen = generation;

      freeIndices.erase(freeIndices.begin() + index);
      entries[index] = entry;
    }

    handle.data = index;
    handle.generation = generation;
    return handle;
  }

  void remove(SGraphicResourceHandle handle) {
    entries[handle.data].valid = false;
    freeIndices.push_back(handle.data);
  }

  T& operator[](uint32 i) {
    if (i < entries.size()) {
      return entries[i];
    }
  }

  std::vector<T> entries;
  std::vector<uint32> freeIndices;
};

//ResourcePool<Dx11PixelShader> g_PixelBufferResources{};
GFXResourcePoll<Dx11PixelShaderEntry> g_EntriesPixelShader{};

SGraphicResourceHandle IDirect3D11::getPixelShader(const wchar* path) {

  Dx11PixelShaderEntry entry{};
  entry.pResource = new Dx11PixelShader;
  entry.pResource->createResources(path);

  return g_EntriesPixelShader.add(entry);
}

void IDirect3D11::setDrawCommand(DrawCommand& drawCommand) {
  Dx11PixelShaderEntry& pixelShaderEntry = g_EntriesPixelShader[drawCommand.pixelShader.data];

  if (pixelShaderEntry.valid) {
    g_EntriesPixelShader[drawCommand.pixelShader.data].pResource->bindResource();
  }
  else {
    printf("Entry is NOT invalid\n");
  }
}
