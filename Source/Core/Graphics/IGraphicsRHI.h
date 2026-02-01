/* ======================================================================================
 *  IGraphicsModule.h
 * 
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Graphics/Vertex.h"
#include "Core/Graphics/Resource/IGraphicResource.h"
#include "Core/Graphics/Resource/GraphicResourceHandle.h"
#include "Core/Graphics/Resource/GraphicPipeline.h"

/*
 * @brief
 * Base class interface for graphics rendering modules.
*/
class IGraphicsRHI
{
public:
  CORE_API virtual ~IGraphicsRHI() {};
  CORE_API virtual void Initialize(void* pWindowHandle) = 0;
  CORE_API virtual void draw() = 0;
  CORE_API virtual void Clean() = 0;

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
  CORE_API virtual VertexBufferHandle   createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount) = 0;
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
  CORE_API virtual IndexBufferHandle    createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) = 0;
  CORE_API virtual VertexShaderHandle   createVertexShader(const char* path) = 0;
  CORE_API virtual FragmentShaderHandle createFragmentShader(const char* path) = 0;
  CORE_API virtual PipelineHandle       createPipeline(SPipelineDesc* pPipelineDesc) = 0;
  CORE_API virtual DepthRTHandle        createDepthRT() = 0;
  CORE_API virtual ConstantBufferHandle createConstantBuffer(uint64 objectByteSize) = 0;


  CORE_API virtual void updateConstantBuffer(ConstantBufferHandle* pConstantBuffer, void* pNewData) = 0;

  /*
   * @brief
   * Frees the allocated resources by the RHI on the GPU. Once freed
   * the handle becomes invalid.
   * 
   * @param handle
   * A handle to the resource to free.
  */
  CORE_API virtual void freeResource(ResourceHandle handle) = 0;

  /*
   * @brief 
   * Bind a vertex buffer to the command buffer.
   * 
   * @param pVertexBufferHandle
   * A pointer to a VertexBufferHandle object.
  */
  CORE_API virtual void cmdBindVertexBuffer(VertexBufferHandle* pVertexBufferHandle) = 0;

  /*
   * @brief 
   * Bind an index buffer to the command buffer.
   * 
   * @param pIndexBufferHandle
   * A pointer to a IndexBufferHandle object.
  */
  CORE_API virtual void cmdBindIndexBuffer(IndexBufferHandle* pIndexBufferHandle) = 0;

  CORE_API virtual void cmdBindVertexShader(VertexShaderHandle* pVertexShaderHandle) = 0;

  CORE_API virtual void cmdBindFragmentShader(FragmentShaderHandle* pFragmentShader) = 0;

  CORE_API virtual void cmdBindPipeline(PipelineHandle* pPipeline) = 0;

  CORE_API virtual void cmdSetRenderTargets(DepthRTHandle* pDepthRTHandle) = 0;

  /*
   * @brief 
   * Add a draw indexed command to the command buffer
   * 
   * @param indexCount
   * The index count of the draw indexed command.
  */
  CORE_API virtual void cmdDrawIndexed(uint32 indexCount) = 0;

  /*
   * @brief
   * Excecutes all the commands in the command buffer.
   * The command buffer gets cleared once all its commands have be ran.
  */
  CORE_API virtual void excecuteCommands() = 0;

  CORE_API virtual void cmdBindConstantBuffer(ConstantBufferHandle* pConstantBuffer) = 0;

  /*
  * OLD SYSTEMS TO BE UPDATED SOON
  */

  CORE_API virtual ITextureResource* createTextureResource(const Image* pImage) = 0;
  CORE_API virtual ISampler* createSamplerResource() = 0;
  CORE_API virtual void BindTexture(ITextureResource* pTexture) = 0;
  CORE_API virtual void bindSampler(ISampler* pSampler) = 0;
};