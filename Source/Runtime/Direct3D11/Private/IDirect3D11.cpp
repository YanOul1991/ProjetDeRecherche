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
#include "Private/Resources/Dx11VertexBuffer.h"
#include "Private/Resources/Dx11IndexBuffer.h"
#include "Private/Resources/Dx11VertexShader.h"
#include "Private/Resources/Dx11PixelShader.h"
#include "Private/Resources/Dx11Texture.h"
#include "Private/Resources/Dx11Sampler.h"

#include "DirectX11Graphics.h"
#include "Runtime/Direct3D11/Dx11RHI.h"

#include <cmath>
#include <vector>

SCommandBuffer Dx11RHI::cmdBuffer{};

extern "C" DIRECTX11_API Dx11RHI* CreateDirect3D11Module() {
  return new Dx11RHI;
}

Dx11RHI::Dx11RHI() :
  m_hTargetWindow{ nullptr },
  m_pBase{ nullptr }
{}

Dx11RHI::~Dx11RHI() {}

void Dx11RHI::Initialize(void* _WindowHandle)
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

void Dx11RHI::draw() {
  /*
  for (SCommand& cmd : cmdBuffer.commands) {
		switch (cmd.type) {
			case ECommandType::drawIndexed: {
        std::cout 
          << "Command type: drawIndexed" << " | Command offset: " << cmd.dataOffset << " | Index count: " << *((uint32*)(cmdBuffer.data.data() + cmd.dataOffset)) << '\n';
        break;
			}
			default:
				break;
		}
  }
  */
  m_pBase->clearBuffer((34.0f / 255.0f), (38.0f / 255.0f), (92.0f / 255.0f), 1.0f);
  m_pBase->renderUpdate();
  excecuteCommands();
  m_pBase->presentBuffer();
}

void Dx11RHI::Clean() {}

ID3D11Device* Dx11RHI::getDevicePtr() {
  return DirectX11Graphics::deviceRef;
}

ID3D11DeviceContext* Dx11RHI::getContextPtr() {
  return DirectX11Graphics::contextRef;
}

// ##################################
//    RESOURCE CREATION FUNCTIONS
// ##################################

IVertexBuffer* Dx11RHI::createVertexBuffer(Vertex* pVertices, const uint32& bufferElementCount) {
  Dx11VertexBuffer* pResource = new Dx11VertexBuffer;
  pResource->createResources(pVertices, bufferElementCount);
  return pResource;
}

IIndexBuffer* Dx11RHI::createIndexBuffer(uint32* pIndices, const uint32& bufferElementCount) {
  Dx11IndexBuffer* pResource = new Dx11IndexBuffer;
  pResource->createResources(pIndices, bufferElementCount);
  return pResource;
}

IVertexShader* Dx11RHI::createVertexShader(const wchar* path) {
  Dx11VertexShader* pResource = new Dx11VertexShader;
  pResource->createResources(path);
  return pResource;
}

IPixelShader* Dx11RHI::createPixelShader(const wchar* path) {
  Dx11PixelShader* pResource = new Dx11PixelShader;
  pResource->createResources(path);
  return pResource;
}

ITextureResource* Dx11RHI::createTextureResource(const Image* pImage) {
  Dx11TextureResource* pResource = new Dx11TextureResource;
  pResource->createResource(pImage);
  return pResource;
}

ISampler* Dx11RHI::createSamplerResource() {
  Dx11Sampler* pResource = new Dx11Sampler;
  pResource->createResource();
  return pResource;
}

// ##################################
//    BINDING FUNCTIONS
// ##################################

void Dx11RHI::bindVertexBuffer(IVertexBuffer* pVertexBuffer) {
  pVertexBuffer->bindResource();
}

void Dx11RHI::bindIndexBuffer(IIndexBuffer* pIndexBuffer) {
  pIndexBuffer->bindResource();
}

void Dx11RHI::bindVertexShader(IVertexShader* pVertexShader) {
  pVertexShader->bindResource();
}

void Dx11RHI::bindPixelShader(IPixelShader* pPixelShader) {
  pPixelShader->bindResource();
}

void Dx11RHI::bindTexture(ITextureResource* pTexture) {
  pTexture->bindResource();
}

void Dx11RHI::bindSampler(ISampler* pSampler) {
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

inline void Dx11RHI::excecuteCommands() 
{
	for (SCommand& cmd : cmdBuffer.commands) {
		switch (cmd.type) {
			case ECommandType::drawIndexed: {
				m_pBase->m_pContext->DrawIndexed(*reinterpret_cast<uint32*>(cmdBuffer.data.data() + cmd.dataOffset), 0, 0);
        break;
			}
      default: {
				break;
      }
		} // switch - END
	} // for - END

  // After executing all commands buffer is cleared
  cmdBuffer.commands.clear();
  cmdBuffer.data.clear();
}

SGraphicResourceHandle Dx11RHI::getPixelShader(const wchar* path) {

  Dx11PixelShaderEntry entry{};
  entry.pResource = new Dx11PixelShader;
  entry.pResource->createResources(path);
  return g_EntriesPixelShader.add(entry);
}

void Dx11RHI::setDrawCommand(DrawCommand& drawCommand) {
  //Dx11PixelShaderEntry& pixelShaderEntry = g_EntriesPixelShader[drawCommand.pixelShader.data];

  //if (pixelShaderEntry.valid) {
  //  g_EntriesPixelShader[drawCommand.pixelShader.data].pResource->bindResource();
  //}
  //else {
  //  printf("Entry is NOT invalid\n");
  //}

  bindVertexBuffer(drawCommand.pVertexBuffer);
  bindIndexBuffer(drawCommand.pIndexBuffer);
  bindVertexShader(drawCommand.pVertexShader);
  bindPixelShader(drawCommand.pPixelShader);
  bindTexture(drawCommand.pTexture);
  bindSampler(drawCommand.pSampler);
  m_pBase->drawCallIndexCount = drawCommand.indexCount;
}

void Dx11RHI::setCommandBuffer(DrawCommand* pDrawCommandBuffer, uint32 count) {
  for (size_t i = 0; i < count; i++) {
    setDrawCommand(pDrawCommandBuffer[i]);
    draw();
  }
}
