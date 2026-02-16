/* ======================================================================================
 *  Dx11RHI.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Runtime/Direct3D11/Dx11RHI.h"

#include "Core/Exception/exception.h"
#include "Core/Input/Input.h"
#include "Core/System/Application.h"
#include "Core/Time/Time.h"
#include "Core/Types/string.h"
#include "Private/Dx11RHIDevice.h"
#include "Private/Dx11ResourceRegistery.h"
#include "Private/Resources/DirectX11Buffer.h"
#include "Private/Resources/Dx11IndexBuffer.h"
#include "Private/Resources/Dx11Pipeline.h"
#include "Private/Resources/Dx11PixelShader.h"
#include "Private/Resources/Dx11Sampler.h"
#include "Private/Resources/Dx11Texture.h"
#include "Private/Resources/Dx11VertexBuffer.h"
#include "Private/Resources/Dx11VertexShader.h"

#include <array>
#include <cmath>
#include <vector>

using byte = unsigned char;

extern "C" DIRECTX11_API Dx11RHI* CreateDirect3D11Module() {
  return new Dx11RHI;
}

static std::wstring towstr(std::string& str) {
  uint32       size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
  std::wstring wstr(size, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
  return wstr;
}

static std::vector<Dx11DepthStencilViewTexture*> g_depthStencilViewTextureResources{};

static GraphicResourceRegistery g_registery{};

CommandBuffer Dx11RHI::cmdBuffer{};

/*
static std::unordered_map<ECommandType, void (*)(void*)>& StaticGraphicsBinding() {
  static std::unordered_map<ECommandType, void (*)(void*)> functions{
    { ECommandType::DrawIndexed,
      [](void* pData) -> void {
        printf("Calling draw from binding: %d\n", *reinterpret_cast<uint32*>(pData));
      }
    }
  };
  return functions;
}
class Dx11Pipeline_old
{
 public:
  VertexShaderHandle     vertexShaderHandle{0};
  FragmentShaderHandle   fragmentShaderHandle{0};
  Dx11DepthStencil*      pDepthStencilState{nullptr};
  Dx11RasterizerState*   pRasterizer{nullptr};
  D3D_PRIMITIVE_TOPOLOGY primitveTopology{D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED};
};
*/

Dx11RHI::Dx11RHI() :
    m_outputWindow{nullptr},
    pDx11RHIDevice{nullptr} {
}

Dx11RHI::~Dx11RHI() {
}

void Dx11RHI::Initialize(void* _WindowHandle) {
  m_outputWindow = _WindowHandle;
  pDx11RHIDevice = new Dx11RHIDevice{};
  if (pDx11RHIDevice) {
    pDx11RHIDevice->initialize(reinterpret_cast<HWND>(m_outputWindow), this);
    pDx11RHIDevice->initRenderTargetView(1920, 1080);
  }
}

// ###########################################################
//    DRAW CALL
// ###########################################################

void Dx11RHI::draw() {
  pDx11RHIDevice->clearBuffer((34.0f / 255.0f), (38.0f / 255.0f), (92.0f / 255.0f), 1.0f);
  pDx11RHIDevice->renderUpdate();
  excecuteCommands();
  pDx11RHIDevice->presentBuffer();
}

void Dx11RHI::Clean() {
}

/**
 * ----------------------------------------------------------------------------
 * OUTPUT WINDOW RESIZING MANAGEMENT
 * ----------------------------------------------------------------------------
 */
void Dx11RHI::updateSystemWindowSize(uint32 param_newWidth, uint32 param_newHeight) {
  OPTIM_WIN_COM_CHECK_START();

  // printf("Will try to  resize swap chain. new Size : (%du, %du)\n", param_newWidth, param_newHeight);
  pDx11RHIDevice->clearRenderTargetView();

  if (pDx11RHIDevice->m_pSwapChain != nullptr) {
    OPTIM_WIN_THROW_ON_FAILED(
      pDx11RHIDevice->m_pSwapChain->ResizeBuffers(0, param_newWidth, param_newHeight, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0));
    // printf("Swap chain resized!\n");
  }
  else {
    printf("[Error]\nCannot find swap chain.\n");
  }

  for (auto& depthBuffer : g_depthStencilViewTextureResources) {
    depthBuffer->resize(pDx11RHIDevice->m_pDevice.Get(), param_newWidth, param_newHeight);
  }

  // printf("Updated all Depth stencils\n");
  pDx11RHIDevice->initRenderTargetView(param_newWidth, param_newHeight);
}

ID3D11Device* Dx11RHI::getDevicePtr() {
  return Dx11RHIDevice::deviceRef;
}

ID3D11DeviceContext* Dx11RHI::getContextPtr() {
  return Dx11RHIDevice::contextRef;
}

ID3D11RenderTargetView* Dx11RHI::initRenderTargetView() {
  return Dx11RHIDevice::renderTargetView;
}

// ##################################
//  OLD RESOURCE CREATION FUNCTIONS
// ##################################

/*
ITextureResource* Dx11RHI::createTextureResource(const Image* pImage) {
  Dx11TextureResource_old* pResource = new Dx11TextureResource_old;
  pResource->createResource(pImage);
  return pResource;
}

ISampler* Dx11RHI::createSamplerResource() {
  Dx11Sampler* pResource = new Dx11Sampler;
  pResource->createResource();
  return pResource;
}

// ##################################
//    OLD BINDING FUNCTIONS
// ##################################

void Dx11RHI::BindTexture(ITextureResource* pTexture) {
  pTexture->bindResource();
}

void Dx11RHI::bindSampler(ISampler* pSampler) {
  pSampler->bindResource();
}
*/

void Dx11RHI::freeResource(ResourceHandle handle) {
  g_registery.freeResource(handle);
}

/**
 * ################################################################
 *    RESOURCE CREATION FUNCTIONS
 * ################################################################
 *
 * Each of the functions that creates a resources does so by allocating
 * memory on the heap for each apporpriate Dx11[XYZ] ressource
 *
 * Each of those functions, then calls the appropriate functions to
 * create the necessary ressources from the passed in paramters.
 *
 * Then they are finally added to a registery and by passing in
 * the type of the ressource and the pointer to the Dx11 object
 * casted as a void*.
 *
 * The function returns a generic ResourceHandle object, where the
 * data value is passed in as the data value for the correct
 * resource handle type.
 *
 * ################################################################
 */

VertexBufferHandle Dx11RHI::createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount) {
  Dx11VertexBuffer* l_pResource = new Dx11VertexBuffer;
  l_pResource->create(pVertices, elementCount);
  return VertexBufferHandle{
    .data = g_registery.registerResource(EResourceTypes::VertexBuffer, l_pResource).data};
}

IndexBufferHandle Dx11RHI::createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) {
  Dx11IndexBuffer* l_pResource = new Dx11IndexBuffer;
  l_pResource->create(pIndices, elementCount);
  return IndexBufferHandle{
    .data = g_registery.registerResource(EResourceTypes::IndexBuffer, l_pResource).data};
}

PipelineHandle Dx11RHI::createPipeline(SPipelineDesc* pPipelineDesc) {
  Dx11Pipeline* pResource = new Dx11Pipeline;
  pResource->create(pDx11RHIDevice->m_pDevice.Get(), *pPipelineDesc);
  return PipelineHandle{
    .data = g_registery.registerResource(EResourceTypes::Pipeline, pResource).data};
}

DepthRTHandle Dx11RHI::createDepthRT() {
  Dx11DepthStencilViewTexture* pResource = new Dx11DepthStencilViewTexture;
  pResource->create(pDx11RHIDevice->m_pDevice.Get());

  g_depthStencilViewTextureResources.push_back(pResource);

  return DepthRTHandle{
    .data = g_registery.registerResource(EResourceTypes::DepthRT, pResource).data};
}

ConstantBufferHandle Dx11RHI::createConstantBuffer(uint64 objectByteSize) {
  Dx11ConstantBuffer* pResource = new Dx11ConstantBuffer;
  pResource->create(pDx11RHIDevice->m_pDevice.Get(), (uint32)objectByteSize);
  return ConstantBufferHandle{
    .data = g_registery.registerResource(EResourceTypes::ConstantBuffer, pResource).data};
}

TextureResourceHandle Dx11RHI::createTextureResource(const Image* pImage) {
  Dx11TextureResource* pResource = new Dx11TextureResource;
  pResource->createResource(pDx11RHIDevice->m_pDevice.Get(), pImage);
  return TextureResourceHandle{
    .data = g_registery.registerResource(EResourceTypes::Texture, pResource).data};
}

// OLD CREATION FUNCTIONS
/*
VertexShaderHandle Dx11RHI::createVertexShader(const char* path) {
  Dx11VertexShader* l_pResource = new Dx11VertexShader;

  std::string str = path;
  std::wstring wstrPath = towstr(str);
  l_pResource->createResources(wstrPath.c_str());

  return VertexShaderHandle{
    .data = g_registery.registerResource(EResourceTypes::VertexShader, l_pResource).data};
}

FragmentShaderHandle Dx11RHI::createFragmentShader(const char* path) {
  Dx11PixelShader* l_pResource = new Dx11PixelShader;

  std::string str = path;
  std::wstring wstrPath = towstr(str);
  l_pResource->createResources(wstrPath.c_str());

  return FragmentShaderHandle{
    .data = g_registery.registerResource(EResourceTypes::FragmentShader, l_pResource).data};
}

PipelineHandle Dx11RHI::createPipeline(SPipelineDesc* param_pipelineDesc) {
  Dx11Pipeline_old* l_pPipeline         = new Dx11Pipeline_old;
  l_pPipeline->vertexShaderHandle   = param_pipelineDesc->vertexShaderHandle;
  l_pPipeline->fragmentShaderHandle = param_pipelineDesc->fragmentShaderHandle;
  l_pPipeline->pDepthStencilState   = new Dx11DepthStencil(pDx11RHIDevice->m_pDevice.Get(), &param_pipelineDesc->depthStencilDescription);
  l_pPipeline->pRasterizer          = new Dx11RasterizerState(&param_pipelineDesc->rasterizerDescription);
  l_pPipeline->primitveTopology     = static_cast<D3D_PRIMITIVE_TOPOLOGY>(static_cast<int>(param_pipelineDesc->primitiveTopology));

  // Add the depth stencil state to its list
  //g_depthStencilResources.push_back(l_pPipeline->pDepthStencilState);

  return PipelineHandle{
    .data = g_registery.registerResource(EResourceTypes::Pipeline, l_pPipeline).data};
}

PipelineHandle Dx11RHI::createPipelineResource(SPipelineDescription* param_pPipelineDesc) {
  Dx11Pipeline* l_pResource = new Dx11Pipeline;
  l_pResource->create(pDx11RHIDevice->m_pDevice.Get(), *param_pPipelineDesc);
  return PipelineHandle{0};
}

DepthRTHandle Dx11RHI::createDepthRT() {
  Dx11DepthStencilViewTexture* l_pDepthRT = new Dx11DepthStencilViewTexture(pDx11RHIDevice->m_pDevice.Get());

  // Add the depth stencil view to its list
  g_depthStencilViewTextureResources.push_back(l_pDepthRT);

  return DepthRTHandle{
    .data = g_registery.registerResource(EResourceTypes::DepthRT, l_pDepthRT).data};
}

ConstantBufferHandle Dx11RHI::createConstantBuffer(uint64 objectByteSize) {
  Dx11ConstantBuffer* l_pConstantBuffer = new Dx11ConstantBuffer(pDx11RHIDevice->m_pDevice.Get(), static_cast<uint32>(objectByteSize));
  return ConstantBufferHandle{
    .data = g_registery.registerResource(EResourceTypes::ConstantBuffer, l_pConstantBuffer).data};
}
*/

/**
 * ################################################################
 *    CONSTANT BUFFER UPDATES
 * ################################################################
 *
 * Constant buffers have a special function that allows them
 * to be updated at any time through the RHI when binded to
 * shader pipelines.
 *
 * ################################################################
 */

void Dx11RHI::updateConstantBuffer(ConstantBufferHandle* pConstantBuffer, void* pNewData) {
  if (!g_registery.validateHandle((ResourceHandle*)pConstantBuffer, EResourceTypes::ConstantBuffer)) {
    printf("The handle is not a constant buffer resource handle or the resource as been destroyed.\n");
    return;
  }
  reinterpret_cast<Dx11ConstantBuffer*>(g_registery[(ResourceHandle*)pConstantBuffer]->pResource)->update(pDx11RHIDevice->m_pContext.Get(), pNewData);
}

/**
 * @brief
 *
 * For binding a pipeline resource, get the address of the registery.
 */
void Dx11RHI::cmdBindPipeline(PipelineHandle* pPipeline) {
  if (!g_registery.validateHandle((ResourceHandle*)pPipeline, EResourceTypes::Pipeline)) {
    printf("The handle is not a PipelineHandle or the resource as been destroyed.\n");
    return;
  }
  // uintptr_t dataAddress = (uintptr_t)g_registery[(ResourceHandle*)pPipeline]->pResource;
  cmdBuffer.push(
    ECommandType::BindPipeline,
    &g_registery[(ResourceHandle*)pPipeline]->pResource,
    sizeof(IDx11Resource*));
}

void Dx11RHI::cmdSetRenderTargets(DepthRTHandle* pDepthRTHandle) {
  if (!g_registery.validateHandle((ResourceHandle*)pDepthRTHandle, EResourceTypes::DepthRT)) {
    printf("The handle is not a DepthRTHandle or the resource as been destroyed.\n");
    return;
  }
  cmdBuffer.push(
    ECommandType::SetRenderTargets,
    &g_registery[(ResourceHandle*)pDepthRTHandle]->pResource,
    sizeof(IDx11Resource*));
}

void Dx11RHI::cmdBindVertexBuffer(VertexBufferHandle* pVertexBufferHandle) {
  if (!g_registery.validateHandle((ResourceHandle*)pVertexBufferHandle, EResourceTypes::VertexBuffer)) {
    printf("The handle is not a VertexBufferHandle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindVertexBuffer,
    &g_registery[(ResourceHandle*)pVertexBufferHandle]->pResource,
    sizeof(IDx11Resource*));
}

void Dx11RHI::cmdBindIndexBuffer(IndexBufferHandle* pIndexBufferHandle) {
  if (!g_registery.validateHandle((ResourceHandle*)pIndexBufferHandle, EResourceTypes::IndexBuffer)) {
    printf("The handle is not an IndexBufferHandle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindIndexBuffer, 
    &g_registery[(ResourceHandle*)pIndexBufferHandle]->pResource, 
    sizeof(IDx11Resource*));
}

void Dx11RHI::cmdBindTexture(TextureResourceHandle* pTextureResourceHandle) {
  if (!g_registery.validateHandle((ResourceHandle*)pTextureResourceHandle, EResourceTypes::Texture)) {
    printf("The handle is not a TextureResourceHandle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindTexture,
    &g_registery[(ResourceHandle*)pTextureResourceHandle]->pResource, 
    sizeof(IDx11Resource*));
}

/**
 * @brief
 *
 * For draw indexed command store the index count
 * of the next draw indexed call.
 */
void Dx11RHI::cmdDrawIndexed(uint32 indexCount) {
  cmdBuffer.push(
    ECommandType::DrawIndexed,
    &indexCount,
    sizeof(indexCount));
}

/**
 * @brief
 *
 * For updating the world transform of the next mesh
 * pass in the tranform matrix of the mesh.
 */
void Dx11RHI::cmdSetNextMeshTransform(float4x4* meshWorldTransform) {
  cmdBuffer.push(
    ECommandType::BindConstantBufferTransformMatrix,
    meshWorldTransform,
    sizeof(*meshWorldTransform));
}

void Dx11RHI::cmdBindConstantBuffer(ConstantBufferHandle* pConstantBuffer) {
  if (!g_registery.validateHandle((ResourceHandle*)pConstantBuffer, EResourceTypes::ConstantBuffer)) {
    printf("The handle is not a ConstantBufferHandle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindConstantBuffer,
    &g_registery[(ResourceHandle*)pConstantBuffer]->pResource,
    sizeof(IDx11Resource*));
}

/**
 * ################################################################
 *    RESOURCE FREEING
 * ################################################################
 */

/**
 * ################################################################
 *    COMMAND BINDING FUNCTIONS
 * ################################################################
 *
 * These functions add an instruction to the command buffer.
 *
 * Each of these recieves the corrsponding resource handle
 * depeneding on the command type.
 *
 * The handle must be validated to confirm that the registery
 * entry's data pointer holds the data to the appropriate resource
 * type
 *
 * Then when added to the command buffer, must pass in the
 * appropriate command type.
 */

/*
void Dx11RHI::cmdBindPipeline(PipelineHandle* pPipeline) {
  if (!g_registery.validateHandle((ResourceHandle*)pPipeline, EResourceTypes::Pipeline)) {
    printf("The handle is not a pipeline resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindPipeline,
    &g_registery[(ResourceHandle*)pPipeline]->pResource,
    sizeof(void*));
}

void Dx11RHI::cmdSetRenderTargets(DepthRTHandle* pDepthRTHandle) {
  if (!g_registery.validateHandle((ResourceHandle*)pDepthRTHandle, EResourceTypes::DepthRT)) {
    printf("The handle is not a DepthRT resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::SetRenderTargets,
    &g_registery[(ResourceHandle*)pDepthRTHandle]->pResource,
    sizeof(void*));
}

void Dx11RHI::cmdBindVertexBuffer(VertexBufferHandle* param_pVertexBufferHandle) {
  if (!g_registery.validateHandle((ResourceHandle*)param_pVertexBufferHandle, EResourceTypes::VertexBuffer)) {
    printf("The handle is not a vertex buffer resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindVertexBuffer,
    &g_registery[(ResourceHandle*)param_pVertexBufferHandle]->pResource,
    sizeof(void*));
}

void Dx11RHI::cmdBindIndexBuffer(IndexBufferHandle* pVertexBufferHandle) {
  if (!g_registery.validateHandle((ResourceHandle*)pVertexBufferHandle, EResourceTypes::IndexBuffer)) {
    printf("The handle is not an index buffer resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindIndexBuffer,
    &g_registery[(ResourceHandle*)pVertexBufferHandle]->pResource,
    sizeof(void*)
  );
}
void Dx11RHI::cmdBindVertexShader(VertexShaderHandle* pVertexShaderHandle) {
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

void Dx11RHI::cmdBindFragmentShader(FragmentShaderHandle* pFragmentShader) {
  if (!g_registery.validateHandle((ResourceHandle*)pFragmentShader, EResourceTypes::FragmentShader)) {
    printf("The handle is not a fragment shader resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindFragmentShader,
    &g_registery[(ResourceHandle*)pFragmentShader]->pResource,
    sizeof(void*));
}

void Dx11RHI::cmdBindConstantBuffer(ConstantBufferHandle* pConstantBuffer) {
  if (!g_registery.validateHandle((ResourceHandle*)pConstantBuffer, EResourceTypes::ConstantBuffer)) {
    printf("The handle is not a constant buffer resource handle or the resource as been destroyed.\n");
    return;
  }

  cmdBuffer.push(
    ECommandType::BindConstantBuffer,
    &g_registery[(ResourceHandle*)pConstantBuffer]->pResource,
    sizeof(void*));
}

void Dx11RHI::cmdDrawIndexed(uint32 param_indexCount) {
  cmdBuffer.push(
    ECommandType::DrawIndexed,
    &param_indexCount,
    sizeof(uint32));
}

void Dx11RHI::cmdSetNextMeshTransform(float4x4* meshWorldTransform) {
  cmdBuffer.push(
    ECommandType::BindConstantBufferTransformMatrix,
    meshWorldTransform,
    sizeof(float4x4));
}

*/

/**
 * ################################################################
 *    COMMAND BUFFER EXCECUTION
 * ################################################################
 */

void Dx11RHI::excecuteCommands() {
  /*
  std::vector<Dx11DepthStencilViewTexture*> l_listDepthRT{};

  for (SCommand& cmd : cmdBuffer.commands) {
    uint8* l_pData = cmdBuffer.data.data() + cmd.dataOffset;

    switch (cmd.type) {
    case ECommandType::DrawIndexed: {
      uint32 l_indexCount = *(reinterpret_cast<uint32*>(l_pData));
      pDx11RHIDevice->m_pContext->DrawIndexed(l_indexCount, 0, 0);
      break;
    }
    case ECommandType::BindVertexBuffer: {
      (*(reinterpret_cast<Dx11VertexBuffer**>(l_pData)))->bindResource();
      break;
    }
    case ECommandType::BindIndexBuffer: {
      (*(reinterpret_cast<Dx11IndexBuffer**>(l_pData)))->bindResource();
      break;
    }
    case ECommandType::BindConstantBuffer: {
      (*reinterpret_cast<Dx11ConstantBuffer**>(l_pData))->bindResource(pDx11RHIDevice->m_pContext.Get());
      break;
    }
    case ECommandType::SetRenderTargets: {
      Dx11DepthStencilViewTexture* l_pDepthRT = (*reinterpret_cast<Dx11DepthStencilViewTexture**>(l_pData));
      l_pDepthRT->bindResource(pDx11RHIDevice->m_pContext.Get(), pDx11RHIDevice->m_pRenderTargetView.GetAddressOf());
      l_listDepthRT.push_back(l_pDepthRT);
      break;
    }
    case ECommandType::BindConstantBufferTransformMatrix: {
      float4x4 transform = *reinterpret_cast<float4x4*>(l_pData);
      pDx11RHIDevice->vsInputConstBufferData.transform = transform.transpose();
      updateConstantBuffer(&pDx11RHIDevice->constantBufferTransformView, &pDx11RHIDevice->vsInputConstBufferData);
      break;
    }
    case ECommandType::BindPipeline: {

    }
    default: {
      break;
    }
    } // switch - END
  } // for - END
  */

  // static int iterations = 10;

  for (auto& cmd : cmdBuffer.commands) {
    //std::cout << "Excecuting command: " << (int)cmd.type << "\n";

    byte* data = cmdBuffer.data.data() + cmd.dataOffset;

    /*
     * Handle unique commands such as drawIndexed and bindConstantBufferTransformMatrix
     */

    if (cmd.type == ECommandType::DrawIndexed) {
      pDx11RHIDevice->m_pContext->DrawIndexed(*reinterpret_cast<uint32*>(data), 0, 0);
      continue;
    }

    if (cmd.type == ECommandType::BindConstantBufferTransformMatrix) {
      float4x4 transform                               = *reinterpret_cast<float4x4*>(data);
      pDx11RHIDevice->vsInputConstBufferData.transform = transform.transpose();
      updateConstantBuffer(&pDx11RHIDevice->constantBufferTransformView, &pDx11RHIDevice->vsInputConstBufferData);
      continue;
    }

    IDx11Resource* ptr = nullptr;

    memcpy(&ptr, data, sizeof(ptr));

    /*
    if (!ptr) {
      printf("Pointer is null\n");
    }
    else {
      std::cout << "Pointer address: " << std::hex << (void*)ptr << std::dec << '\n';
    }
    */

    ptr->bind(pDx11RHIDevice->m_pContext.Get(), pDx11RHIDevice->m_pRenderTargetView.GetAddressOf());
  }

  // Clear all depth stencil view/texture rssources
  for (Dx11DepthStencilViewTexture*& pDepthRT : g_depthStencilViewTextureResources) {
    pDepthRT->clearDepthStencilView(pDx11RHIDevice->m_pContext.Get());
  }

  // After executing all commands buffer is cleared
  cmdBuffer.commands.clear();
  cmdBuffer.data.clear();
}