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
#include "Private/Resources/Dx11ConstantBuffer.h"
#include "Private/Resources/Dx11DepthStencilView.h"
#include "Private/Resources/Dx11IndexBuffer.h"
#include "Private/Resources/Dx11Pipeline.h"
#include "Private/Resources/Dx11Texture.h"
#include "Private/Resources/Dx11VertexBuffer.h"

#include <array>
#include <cmath>
#include <vector>

extern "C" DIRECTX11_API Dx11RHI* CreateDirect3D11Module() {
  return new Dx11RHI;
}

std::vector<uint32> Dx11RHI::staticActivePipelineInputs{};

static std::wstring towstr(std::string& str) {
  uint32       size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
  std::wstring wstr(size, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
  return wstr;
}

static std::vector<Dx11DepthStencilViewTexture*> g_depthStencilViewTextureResources{};

static GraphicResourceRegistery g_registery{};

CommandBuffer Dx11RHI::cmdBuffer{};

std::vector<uint32>& Dx11RHI::getActivePipelineInputs() {
  return staticActivePipelineInputs;
}

void Dx11RHI::StaticUpdateActivePipelineInputs(uint32 inputMask) {
  static bool init = false;

  staticActivePipelineInputs.clear();

  uint32 _mask = inputMask;

  while (_mask) {
    // __builtin_ctz for unix ?
    uint32 i = _tzcnt_u32(_mask);
    staticActivePipelineInputs.push_back(i);
    _mask &= _mask - 1;
  }

  //for (uint32 i = 0; i < sizeof(inputMask) * 8; i++) {
  //  if (((inputMask >> i) & 1)) {
  //    staticActivePipelineInputs.push_back(i);
  //  }
  //}

  //if (!init) {
  //  std::cout << "[Dx11RHI] Checking active pipeline with _tzcnt_u32 mask: \n";
  //  for (auto& i : staticActivePipelineInputs) {
  //    std::cout << "    " << i << '\n';
  //  }
  //  init = true;
  //}
}

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

  pDx11RHIDevice->clearRenderTargetView();

  if (pDx11RHIDevice->m_pSwapChain != nullptr) {
    OPTIM_WIN_THROW_ON_FAILED(
      pDx11RHIDevice->m_pSwapChain->ResizeBuffers(0, param_newWidth, param_newHeight, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0));
  }
  else {
    printf("[Error]\nCannot find swap chain.\n");
  }

  for (auto& depthBuffer : g_depthStencilViewTextureResources) {
    depthBuffer->resize(pDx11RHIDevice->m_pDevice.Get(), param_newWidth, param_newHeight);
  }

  pDx11RHIDevice->initRenderTargetView(param_newWidth, param_newHeight);
}

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
  l_pResource->create(pDx11RHIDevice->m_pDevice.Get(), pVertices, elementCount);
  return VertexBufferHandle{
    .data = g_registery.registerResource(EResourceTypes::VertexBuffer, l_pResource).data};
}

IndexBufferHandle Dx11RHI::createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) {
  Dx11IndexBuffer* l_pResource = new Dx11IndexBuffer;
  l_pResource->create(pDx11RHIDevice->m_pDevice.Get(), pIndices, elementCount);
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
  pResource->create(pDx11RHIDevice->m_pDevice.Get(), pImage);
  return TextureResourceHandle{
    .data = g_registery.registerResource(EResourceTypes::Texture, pResource).data};
}

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
 * @brief
 * Excecute all commands in the buffer.
 * Handles some special commands such as drawindexed
 * and constant buffer updates.
 */
void Dx11RHI::excecuteCommands() {
  for (auto& cmd : cmdBuffer.commands) {
    uint8* data = cmdBuffer.data.data() + cmd.dataOffset;

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
    ptr->bind(pDx11RHIDevice->m_pContext.Get(), pDx11RHIDevice->m_pRenderTargetView.GetAddressOf());
  }

  for (Dx11DepthStencilViewTexture*& pDepthRT : g_depthStencilViewTextureResources) {
    pDepthRT->clearDepthStencilView(pDx11RHIDevice->m_pContext.Get());
  }

  cmdBuffer.commands.clear();
  cmdBuffer.data.clear();
}