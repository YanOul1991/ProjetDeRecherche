/* ======================================================================================
 *  IGraphicsRHI.h
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Graphics/IGraphicsDevice.h"
#include "Core/Graphics/Resource/GraphicPipeline.h"
#include "Core/Graphics/Resource/GraphicResourceHandle.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Object/Image/Image.h"
#include "Core/OptimEngine.h"

/*
 * @brief
 * Base class interface for graphics rendering modules.
 */
class CORE_API IGraphicsRHI
{
 public:
  virtual ~IGraphicsRHI() {};
  virtual void Initialize(void* pWindowHandle) = 0;
  virtual void draw()                          = 0;
  virtual void Clean()                         = 0;

  /*
   * @brief
   * Updates the draw size of the render target, to update rendering settings.
   *
   * @param newWidth
   * The new width of the window.
   *
   * @param newHeight
   * The new height of the window.
   */
  virtual void updateSystemWindowSize(uint32 newWidth, uint32 newHeight) = 0;

  /*
   * @brief
   * Creates and allocates a vertex buffer resources on the GPU.
   *
   * @param pVertices
   * Pointer to a vertex buffer.
   *
   * @param elementCount
   * Number of elements in the buffer.
   *
   * @return
   * A handle to the graphique resource.
   */
  virtual VertexBufferHandle createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount) = 0;

  /*
   * @brief
   * Creates and allocates an index buffer resources on the GPU.
   *
   * @param pIndices
   * Pointer to an index buffer.
   *
   * @param elementCount
   * Number of elements in the buffer.
   *
   * @return
   * A handle to the graphique resource.
   */
  virtual IndexBufferHandle createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) = 0;

  virtual DepthRTHandle createDepthRT() = 0;

  virtual TextureResourceHandle createTextureResource(const Image* pImage) = 0;

  virtual ConstantBufferHandle createConstantBuffer(uint64 objectByteSize) = 0;

  virtual ConstantBufferHandle createConstantBuffer(SCBufferDesc* pDesc) = 0;

  virtual PipelineHandle createPipeline(SPipelineDesc* pPipelineDesc) = 0;

  virtual void updateConstantBuffer(ConstantBufferHandle* pConstantBuffer, void* pNewData) = 0;

  virtual void cmdSetNextMeshTransform(float4x4* meshWorldTransform) = 0;

  /*
   * @brief
   * Frees the allocated resources by the RHI on the GPU. Once freed
   * the handle becomes invalid.
   *
   * @param handle
   * A handle to the resource to free.
   */
  virtual void freeResource(ResourceHandle handle) = 0;

  /*
   * @brief
   * Bind a vertex buffer to the command buffer.
   *
   * @param pVertexBufferHandle
   * A pointer to a VertexBufferHandle object.
   */
  virtual void cmdBindVertexBuffer(VertexBufferHandle* pVertexBufferHandle) = 0;

  /*
   * @brief
   * Bind an index buffer to the command buffer.
   *
   * @param pIndexBufferHandle
   * A pointer to a IndexBufferHandle object.
   */
  virtual void cmdBindIndexBuffer(IndexBufferHandle* pIndexBufferHandle) = 0;

  virtual void cmdBindPipeline(PipelineHandle* pPipeline) = 0;

  virtual void cmdSetRenderTargets(DepthRTHandle* pDepthRTHandle) = 0;

  /*
   * @brief
   * Add a draw indexed command to the command buffer
   *
   * @param indexCount
   * The index count of the draw indexed command.
   */
  virtual void cmdDrawIndexed(uint32 indexCount) = 0;

  virtual void cmdBindConstantBuffer(ConstantBufferHandle* pConstantBuffer) = 0;

  virtual void cmdBindTexture(TextureResourceHandle* pTextureResourceHandle) = 0;

  /*
   * @brief
   * Excecutes all the commands in the command buffer.
   * The command buffer gets cleared once all its commands have be ran.
   */
  virtual void excecuteCommands() = 0;
};