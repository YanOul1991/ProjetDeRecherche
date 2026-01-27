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

#include "Private/Dx11ResourceManagement.h"

CommandBuffer Dx11RHI::cmdBuffer{};

static Optim::RHI::Directx11::GraphicResourceRegistery g_registery{};

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

inline void Dx11RHI::cmdBindVertexBuffer(ResourceHandle* phandle) {
  if (!g_registery.validate(phandle)) {
    printf("The resource handle is invalid\n");
    return;
  }

  if (g_registery.entries[g_registery.getHandleIndex(phandle)].resourceType != EResourceTypes::vertexBuffer) {
    printf("The handle is not a vertex resource handle.\n");
    return;
  }

  printf("binding Vertex buffer.\n");

  cmdBuffer.push(ECommandType::bindVertexBuffer, phandle, sizeof(*phandle));
}

// ##################################
//    RESOURCE CREATION FUNCTIONS
// ##################################

inline ResourceHandle Dx11RHI::createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount)
{
  Dx11VertexBuffer* l_pResource = new Dx11VertexBuffer;
  l_pResource->createResources(pVertices, elementCount);
  return g_registery.registerResource(EResourceTypes::vertexBuffer, l_pResource);
}

inline ResourceHandle Dx11RHI::createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) {
  Dx11IndexBuffer* l_pResource = new Dx11IndexBuffer;
  l_pResource->createResources(pIndices, elementCount);
  return g_registery.registerResource(EResourceTypes::indexbuffer, l_pResource);
}

void Dx11RHI::freeResource(ResourceHandle handle) {
  g_registery.freeResource(handle);
}

inline void Dx11RHI::excecuteCommands()
{
	for (SCommand& cmd : cmdBuffer.commands) {
		switch (cmd.type) {
			case ECommandType::drawIndexed: {
        m_pBase->m_pContext->DrawIndexed(*(reinterpret_cast<uint32*>(cmdBuffer.data.data() + cmd.dataOffset)), 0, 0);
        break;
			}
      case ECommandType::bindVertexBuffer: {
        reinterpret_cast<ResourceHandle*>(cmdBuffer.data.data() + cmd.dataOffset);
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


/*
SGraphicResourceHandle Dx11RHI::createResourceVertexShader(const wchar* path) {
  return SGraphicResourceHandle();
}

SGraphicResourceHandle Dx11RHI::createResourceVertexBuffer(Vertex* pVertexBuffer, const uint32& bufferSize) {
  return SGraphicResourceHandle();
}
void Dx11RHI::setDrawCommand(DrawCommand& drawCommand) {
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
*/
