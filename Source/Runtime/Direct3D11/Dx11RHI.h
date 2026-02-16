/* ======================================================================================
 *  Dx11RHI.h:
 *    Entry point for D3D11 API implementations.
 *
 *  By:
 *    Yanis Oulmane
 *
 * --------------------------------------------------------------------------------------
 *
 *  REFERENCES
 *    References from Microsoft documentation for DIRECTX11 and DXGI.
 *      DXGI    - https://learn.microsoft.com/en-us/windows/win32/api/_direct3ddxgi/
 *      D3D11   - https://learn.microsoft.com/en-us/windows/win32/api/d3d11/
 *
====================================================================================== */

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/Graphics/Resource/IGraphicResource.h"
#include "Core/OptimEngine.h"

#include <iostream>
#include <vector>

class Dx11RHIDevice;

enum class ECommandType {
  BindPipeline,
  BindVertexBuffer,
  BindIndexBuffer,
  BindTexture,
  DrawIndexed,
  BindVertexShader,
  BindFragmentShader,
  SetRenderTargets,
  BindConstantBuffer,
  BindConstantBufferTransformMatrix
};

struct SCommand {
  ECommandType type;
  uint32       dataOffset;
};

class CommandBuffer
{
 public:
  /*
   * @brief
   * Add a command to the command buffer.
   *
   * @param param_cmd_Type
   * The command type from the ECommandType class enum.
   *
   * @param param_pData
   * A pointer to the required data to bind the resource.
   * For a drawIndex command pass a pointer to an index count.
   * For a pipeline pass a pointer to a pipeline object (to be created).
   * For a resource binding a pointer to a ResourceHandle object.
   *
   * @param the byte size if the data passed in the
   *
   */
  void push(ECommandType param_cmdType, void* param_pData, uint32 param_dataSize) {
    // Create a new command structure
    // Assign the type of the data and
    // the offset of the data to be placed
    // inside the data buffer.

    SCommand cmd{};
    cmd.type       = param_cmdType;
    cmd.dataOffset = (uint32)data.size();

    commands.push_back(cmd);
    data.insert(data.end(), reinterpret_cast<uint8*>(param_pData), reinterpret_cast<uint8*>(param_pData) + param_dataSize);
  }

  std::vector<SCommand> commands;
  std::vector<uint8>    data;
};

class Dx11RHI final : public IGraphicsRHI
{
 public:
  static ID3D11Device*           getDevicePtr();
  static ID3D11DeviceContext*    getContextPtr();
  static ID3D11RenderTargetView* initRenderTargetView();

  Dx11RHI();

  /**
   * @brief
   * Class deconstructor.
   */
  virtual ~Dx11RHI() override final;

  /**
   * @brief
   * Initalized the appropriate resources to start using the DirectX11
   * API.
   *
   * @param windowOutput
   * A pointer to an OS window, for DirectX11 the windows
   * the window pointer is of type HWND.
   */
  virtual void Initialize(void* windowOutput) override final;

  /**
   * @brief
   * Start performing all the logic to render the next frame, by
   * such as clearing the backbuffer, getting the rendering camera's
   * states, and reading from the command list.
   */
  virtual void draw() override final;

  /**
   * @brief
   * Performs cleaning operations after rendering a frame.
   * For now does nothing.
   */
  virtual void Clean() override final;

  /**
   * @brief
   * This function updates the information involving about the output window's
   * dimensions. It should clear the backbuffer and updates its width and height
   * data.
   *
   * To make this function work, all DirectX11 resources that hold references
   * to the backbuffer need to be freed, in order for the backbuffer to be allowed
   * to be changed by the DirectX11 API.
   */
  virtual void updateSystemWindowSize(uint32 newWidth, uint32 newHeight) override final;

  /**
   * @brief
   * [THIS FUNCTION WILL BE REPLACED]
   */
  virtual ITextureResource* createTextureResource(const Image* pImage) override final;

  /**
   * @brief
   */
  virtual ISampler* createSamplerResource() override final;

  /**
   * @brief
   * [THIS FUNCTION WILL BE REPLACED]
   */
  virtual void BindTexture(ITextureResource* pTexture) override final;

  /**
   * @brief
   * [THIS FUNCTION WILL BE REPLACED]
   */
  virtual void bindSampler(ISampler* pSampler) override final;

  /**
   * @brief
   * Creates a ID3D11Buffer ressource used for vertices
   *
   * @param pVertices
   * A pointer to a Vertex struct object buffer.
   *
   * @param elementCount the number of vertices in the buffer.
   *
   * @return
   * A handle ressource handle to a vertex buffer resource
   */
  virtual VertexBufferHandle createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount) override final;

  /**
   * @brief
   * Creates a ID3D11Buffer ressource used for indices
   *
   * @param pIndices
   * A pointer to a index buffer.
   *
   * @param elementCount the number of indices in the buffer.
   *
   * @return
   * A handle ressource handle to an index buffer resource
   */
  virtual IndexBufferHandle createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) override final;

  /**
   * @brief
   * Creates a ID3D11VertexShader ressource
   *
   * @param path
   * The path to the compiled .cso object containing the compiled
   * vertex shader data.
   *
   * @return
   * A handle ressource handle to a vertex shader resource
   */
  virtual VertexShaderHandle createVertexShader(const char* path) override final;

  /**
   * @brief
   * Creates a ID3D11PixelShader ressource.
   *
   * @param path
   * The path to the compiled .cso object containing the compiled
   * pixel shader data.
   *
   * @return
   * A handle ressource handle to a pixel shader resource
   */
  virtual FragmentShaderHandle createFragmentShader(const char* path) override final;

  virtual PipelineHandle createPipeline(SPipelineDesc* pPipelineDesc) override final;

  virtual PipelineHandle createPipelineResource(SPipelineDescription* pPipelineDesc) override final;

  virtual DepthRTHandle createDepthRT() override final;

  virtual ConstantBufferHandle createConstantBuffer(uint64 objectByteSize) override final;

  virtual void updateConstantBuffer(ConstantBufferHandle* pConstantBuffer, void* pNewData) override final;

  virtual void cmdBindPipeline(PipelineHandle* pPipeline) override final;
  virtual void cmdSetRenderTargets(DepthRTHandle* pDepthRTHandle) override final;
  virtual void cmdBindVertexBuffer(VertexBufferHandle* pVertexBufferHandle) override final;
  virtual void cmdBindIndexBuffer(IndexBufferHandle* pIndexBufferHandle) override final;
  virtual void cmdBindVertexShader(VertexShaderHandle* pVertexShaderHandle) override final;
  virtual void cmdBindFragmentShader(FragmentShaderHandle* pFragmentShader) override final;
  virtual void cmdDrawIndexed(uint32 indexCount) override final;
  virtual void cmdSetNextMeshTransform(float4x4* meshWorldTransform) override final;

  virtual void cmdBindConstantBuffer(ConstantBufferHandle* pConstantBuffer) override final;

  virtual void excecuteCommands() override final;
  virtual void freeResource(ResourceHandle handle) override final;

 private:
  void*                m_outputWindow; // Target Window.
  Dx11RHIDevice*       pDx11RHIDevice;
  static CommandBuffer cmdBuffer;
};

extern "C" DIRECTX11_API Dx11RHI* CreateDirect3D11Module();