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

#include "./Private/Dx11CommandBuffer.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/OptimEngine.h"

#include <iostream>
#include <vector>

class Dx11RHIDevice;

class Dx11RHI final : public IGraphicsRHI
{
 public:
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

  virtual VertexBufferHandle createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount) override final;

  virtual IndexBufferHandle createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) override final;

  virtual PipelineHandle createPipeline(SPipelineDesc* pPipelineDesc) override final;

  virtual DepthRTHandle createDepthRT() override final;

  virtual ConstantBufferHandle createConstantBuffer(uint64 objectByteSize) override final;

  virtual TextureResourceHandle createTextureResource(const Image* pImage) override final;

  virtual void updateConstantBuffer(ConstantBufferHandle* pConstantBuffer, void* pNewData) override final;

  virtual void cmdBindPipeline(PipelineHandle* pPipeline) override final;

  virtual void cmdSetRenderTargets(DepthRTHandle* pDepthRTHandle) override final;

  virtual void cmdBindVertexBuffer(VertexBufferHandle* pVertexBufferHandle) override final;

  virtual void cmdBindIndexBuffer(IndexBufferHandle* pIndexBufferHandle) override final;

  virtual void cmdBindTexture(TextureResourceHandle* pTextureResourceHandle) override final;

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