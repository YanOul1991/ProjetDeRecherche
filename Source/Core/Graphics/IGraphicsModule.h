/* ======================================================================================
 *  IGraphicsModule.h
 * 
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Utilities/Random/Random.h"

#include "Core/Graphics/Vertex.h"
#include "Core/Graphics/Mesh.h"

#include "Core/Graphics/Resource/IGraphicResource.h"
#include "Core/Graphics/Resource/IVertexBuffer.h"
#include "Core/Graphics/Resource/IIndexBuffer.h"
#include "Core/Graphics/Resource/IVertexShader.h"
#include "Core/Graphics/Resource/IPixelShader.h"
#include "Core/Graphics/Resource/ITextureResource.h"
#include "Core/Graphics/Resource/ISampler.h"

struct DrawCommand {
  SGraphicResourceHandle pixelShader;
};

/*
 * @brief
 * Base class interface for graphics rendering modules
*/
class IGraphicsModule
{
public:
  CORE_API virtual ~IGraphicsModule() {};
  CORE_API virtual void Initialize(void* _WindowHandle) = 0;
  CORE_API virtual void draw() = 0;
  CORE_API virtual void Clean() = 0;

  CORE_API virtual IVertexBuffer*     createVertexBuffer(Vertex* pVertices, const uint32& bufferElementCount) = 0;
  CORE_API virtual IIndexBuffer*      createIndexBuffer(uint32* pIndices, const uint32& bufferElementCount) = 0;
  CORE_API virtual IVertexShader*     createVertexShader(const wchar* path) = 0;
  CORE_API virtual IPixelShader*      createPixelShader(const wchar* path) = 0;
  CORE_API virtual ITextureResource*  createTextureResource(const Image* pImage) = 0;
  CORE_API virtual ISampler*          createSamplerResource() = 0;

  CORE_API virtual void bindVertexBuffer(IVertexBuffer* pVertexBuffer) = 0;
  CORE_API virtual void bindIndexBuffer(IIndexBuffer* pIndexBuffer) = 0;
  CORE_API virtual void bindVertexShader(IVertexShader* pVertexShader) = 0;
  CORE_API virtual void bindPixelShader(IPixelShader* pPixelShader) = 0;
  CORE_API virtual void bindTexture(ITextureResource* pTexture) = 0;
  CORE_API virtual void bindSampler(ISampler* pSampler) = 0;


  /* TESTING FUNCTIONS */
  CORE_API virtual SGraphicResourceHandle getPixelShader(const wchar* path) = 0;

  CORE_API virtual void setDrawCommand(DrawCommand& drawCommand) = 0;
};