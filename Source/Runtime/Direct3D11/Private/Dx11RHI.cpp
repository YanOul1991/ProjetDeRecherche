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
#include "Private/Dx11ResourceManagement.h"
#include "Private/Resources/Buffer/DirectX11Buffer.h"
#include "Private/Dx11RHIDevice.h"

#include "Runtime/Direct3D11/Dx11RHI.h"

#include <cmath>
#include <vector>


struct Dx11Pipeline
{
  VertexShaderHandle    vertexShaderHandle;
  FragmentShaderHandle  fragmentShaderHandle;
  Dx11DepthStencil*     pDepthStencilState;
  Dx11RasterizerState*  pRasterizer{};
};

CommandBuffer Dx11RHI::cmdBuffer{};

static Optim::RHI::Directx11::GraphicResourceRegistery g_registery{};

extern "C" DIRECTX11_API Dx11RHI* CreateDirect3D11Module() {
  return new Dx11RHI;
}

Dx11RHI::Dx11RHI() :
  m_hTargetWindow{ nullptr },
  pDx11RHIDevice{ nullptr }
{}

Dx11RHI::~Dx11RHI() {}

void Dx11RHI::Initialize(void* _WindowHandle)
{
  m_hTargetWindow =_WindowHandle;
  pDx11RHIDevice = new Dx11RHIDevice{};
  if (pDx11RHIDevice) { 
    pDx11RHIDevice->initialize(reinterpret_cast<HWND>(m_hTargetWindow));
  }
}

// ##################################
//    DRAW CALL
// ##################################

void Dx11RHI::draw() 
{
  pDx11RHIDevice->clearBuffer((34.0f / 255.0f), (38.0f / 255.0f), (92.0f / 255.0f), 1.0f);
  pDx11RHIDevice->renderUpdate();
  excecuteCommands();
  pDx11RHIDevice->presentBuffer();
}

void Dx11RHI::Clean() {}

ID3D11Device* Dx11RHI::getDevicePtr() {
  return Dx11RHIDevice::deviceRef;
}

ID3D11DeviceContext* Dx11RHI::getContextPtr() {
  return Dx11RHIDevice::contextRef;
}

ID3D11RenderTargetView* Dx11RHI::getRenderTargetView() {
  return Dx11RHIDevice::renderTargetView;
}

// ##################################
//    RESOURCE CREATION FUNCTIONS
// ##################################

/*
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
*/


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

/*
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
*/

void Dx11RHI::bindTexture(ITextureResource* pTexture) {
  pTexture->bindResource();
}

void Dx11RHI::bindSampler(ISampler* pSampler) {
  pSampler->bindResource();
}

// ##################################
//    RESOURCE CREATION FUNCTIONS
// ##################################

VertexBufferHandle Dx11RHI::createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount)
{
  Dx11VertexBuffer* l_pResource = new Dx11VertexBuffer;

  l_pResource->createResources(pVertices, elementCount);

  return VertexBufferHandle{
    .data = g_registery.registerResource(EResourceTypes::vertexBuffer, l_pResource).data
  };
}

IndexBufferHandle Dx11RHI::createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) {
  Dx11IndexBuffer* l_pResource = new Dx11IndexBuffer;

  l_pResource->createResources(pIndices, elementCount);

  return IndexBufferHandle {
    .data = g_registery.registerResource(EResourceTypes::indexbuffer, l_pResource).data
  };
}

VertexShaderHandle Dx11RHI::createVertexShader(const char* path)
{
  Dx11VertexShader* l_pResource = new Dx11VertexShader;

  uint64 size = MultiByteToWideChar(CP_UTF8, 0, path, -1, 0, 0);
  wchar* l_wstr = new wchar[size];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, l_wstr, size);
  
  l_pResource->createResources(l_wstr);
  delete[] l_wstr;

  return VertexShaderHandle{
    .data = g_registery.registerResource(EResourceTypes::VertexShader, l_pResource).data
  };
}

FragmentShaderHandle Dx11RHI::createFragmentShader(const char* path)
{
  Dx11PixelShader* l_pResource = new Dx11PixelShader;
  uint64 size = MultiByteToWideChar(CP_UTF8, 0, path, -1, 0, 0);
  wchar* l_wstr = new wchar[size];
  MultiByteToWideChar(CP_UTF8, 0, path, -1, l_wstr, size);
  
  l_pResource->createResources(l_wstr);
  delete[] l_wstr;

  return FragmentShaderHandle{
    .data = g_registery.registerResource(EResourceTypes::FragmentShader, l_pResource).data
  };
}

PipelineHandle Dx11RHI::createPipeline(SPipelineDesc* param_pipelineDesc)
{
  Dx11Pipeline* l_pPipeline = new Dx11Pipeline;
  l_pPipeline->vertexShaderHandle     = param_pipelineDesc->vertexShaderHandle;
  l_pPipeline->fragmentShaderHandle   = param_pipelineDesc->fragmentShaderHandle;
  l_pPipeline->pDepthStencilState     = new Dx11DepthStencil;
  l_pPipeline->pRasterizer            = new Dx11RasterizerState;

  return PipelineHandle{
    .data = g_registery.registerResource(EResourceTypes::Pipeline, l_pPipeline).data
  };
}

void Dx11RHI::freeResource(ResourceHandle handle) {
  g_registery.freeResource(handle);
}

// ##################################
//    BINDING FUNCTIONS
// ##################################

void Dx11RHI::cmdBindPipeline(PipelineHandle* pPipeline)
{
  if (!g_registery.validateHandle((ResourceHandle*)pPipeline, EResourceTypes::Pipeline)) {
    printf("The handle is not a pipeline resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindPipeline,
    &g_registery[(ResourceHandle*)pPipeline]->pResource,
    sizeof(void*)
  );
}

void Dx11RHI::cmdBindVertexBuffer(VertexBufferHandle* param_pVertexBufferHandle)
{
  if (!g_registery.validateHandle((ResourceHandle*)param_pVertexBufferHandle, EResourceTypes::vertexBuffer)) {
    printf("The handle is not a vertex buffer resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::bindVertexBuffer, 
    &g_registery[(ResourceHandle*)param_pVertexBufferHandle]->pResource, 
    sizeof(void*)
  );
}

void Dx11RHI::cmdBindIndexBuffer(IndexBufferHandle* pVertexBufferHandle) 
{
  if (!g_registery.validateHandle((ResourceHandle*)pVertexBufferHandle, EResourceTypes::indexbuffer)) {
    printf("The handle is not an index buffer resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::bindIndexBuffer, 
    &g_registery[(ResourceHandle*)pVertexBufferHandle]->pResource, 
    sizeof(void*)
  );
}

void Dx11RHI::cmdBindVertexShader(VertexShaderHandle* pVertexShaderHandle)
{
	if (!g_registery.validateHandle((ResourceHandle*)pVertexShaderHandle, EResourceTypes::VertexShader)) {
		printf("The handle is not a vertex resource handle or the resource as been destroyed.\n");
		return;
	}

  cmdBuffer.push(
    ECommandType::BindVertexShader,
    &g_registery[(ResourceHandle*)pVertexShaderHandle]->pResource,
    sizeof(void*)
  );
}

void Dx11RHI::cmdBindFragmentShader(FragmentShaderHandle* pFragmentShader)
{
  if (!g_registery.validateHandle((ResourceHandle*)pFragmentShader, EResourceTypes::FragmentShader)) {
    printf("The handle is not a fragment shader resource handle or the resource as been destroyed.\n");
		return;
  }

  cmdBuffer.push(
    ECommandType::BindFragmentShader,
    &g_registery[(ResourceHandle*)pFragmentShader]->pResource,
    sizeof(void*)
  );
}

void Dx11RHI::cmdDrawIndexed(uint32 param_indexCount) {
  cmdBuffer.push(ECommandType::drawIndexed, &param_indexCount, sizeof(uint32));
}

/*
 * For directX11
*/
void Dx11RHI::excecuteCommands()
{
  std::vector<Dx11Pipeline*> l_listPipelinesHandles{};

	for (SCommand& cmd : cmdBuffer.commands) {
    uint8* l_pData = cmdBuffer.data.data() + cmd.dataOffset;

		switch (cmd.type) {
			case ECommandType::drawIndexed: {
        uint32 l_indexCount = *(reinterpret_cast<uint32*>(l_pData));
        pDx11RHIDevice->m_pContext->DrawIndexed(l_indexCount, 0, 0);
        break;
			}
      case ECommandType::bindVertexBuffer: {
        (*(reinterpret_cast<Dx11VertexBuffer**>(l_pData)))->bindResource();
        break;
      }
      case ECommandType::bindIndexBuffer: {
        (*(reinterpret_cast<Dx11IndexBuffer**>(l_pData)))->bindResource();
        break;
      }
      case ECommandType::BindVertexShader: {
        (*(reinterpret_cast<Dx11VertexShader**>(l_pData)))->bindResource();
        break;
      }
      case ECommandType::BindFragmentShader: {
        (*(reinterpret_cast<Dx11PixelShader**>(l_pData)))->bindResource();
        break;
      }
      case ECommandType::BindPipeline: {
        Dx11Pipeline* pPipeline = (*reinterpret_cast<Dx11Pipeline**>(l_pData));

        ((Dx11VertexShader*)g_registery[(ResourceHandle*)(&pPipeline->vertexShaderHandle)]->pResource)->bindResource();
        ((Dx11PixelShader*)g_registery[(ResourceHandle*)(&pPipeline->fragmentShaderHandle)]->pResource)->bindResource();
        pPipeline->pDepthStencilState->bind(pDx11RHIDevice->m_pRenderTargetView.GetAddressOf());
        pPipeline->pRasterizer->bind();

        l_listPipelinesHandles.push_back(pPipeline);
        break;
      }
      default: {
				break;
      }
		} // switch - END
	} // for - END

  for (Dx11Pipeline*& pPipeline : l_listPipelinesHandles) {
    pDx11RHIDevice->m_pContext->ClearDepthStencilView(pPipeline->pDepthStencilState->pView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
  }

  // After executing all commands buffer is cleared
  cmdBuffer.commands.clear();
  cmdBuffer.data.clear();
}