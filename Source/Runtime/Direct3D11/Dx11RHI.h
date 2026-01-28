/* ======================================================================================
 *  IDirect3D11.h:
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

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/Graphics/Resource/IGraphicResource.h"
#include <iostream>
#include <vector>

class DirectX11Graphics;

enum class ECommandType {
  bindPipeline,
  bindVertexBuffer,
  bindIndexBuffer,
  bindTexture,
  drawIndexed,

  BindVertexShader
};

struct SCommand {
  ECommandType type;
  uint32 dataOffset;
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
    SCommand cmd{};
    cmd.type = param_cmdType;
    cmd.dataOffset = (uint32)data.size();
    commands.push_back(cmd);
    data.insert(data.end(), reinterpret_cast<uint8*>(param_pData), reinterpret_cast<uint8*>(param_pData) + param_dataSize);
  }

  std::vector<SCommand> commands;
  std::vector<uint8> data;
};

class Dx11RHI final : public IGraphicsRHI 
{
public:
  static ID3D11Device* getDevicePtr();
  static ID3D11DeviceContext* getContextPtr();

  Dx11RHI();
  ~Dx11RHI() override final;
  void Initialize(void* _WindowHandle) override final;
  void draw() override final;
  void Clean() override final;

  /*
  * OLD RESOURC CREATION SYSTEM
  IVertexBuffer*    createVertexBuffer(Vertex* pVertices, const uint32& bufferElementCount) override final;
  IIndexBuffer*     createIndexBuffer(uint32* pIndices, const uint32& bufferElementCount) override final;
  IVertexShader*    createVertexShader(const wchar* path) override final;
  */

  IPixelShader*     createPixelShader(const wchar* path) override final;
  ITextureResource* createTextureResource(const Image* pImage) override final;
  ISampler*         createSamplerResource() override final;

  virtual void bindVertexBuffer(IVertexBuffer* pVertexBuffer) override final;
  virtual void bindIndexBuffer(IIndexBuffer* pIndexBuffer) override final;
  virtual void bindVertexShader(IVertexShader* pVertexShader) override final;
  virtual void bindPixelShader(IPixelShader* pPixelShader) override final;
  virtual void bindTexture(ITextureResource* pTexture) override final;
  virtual void bindSampler(ISampler* pSampler) override final;

  // NEW VIRTUAL FUNCTIONS TO TEST

  virtual VertexBufferHandle createResourceVertexBuffer(Vertex* pVertices, const uint32 elementCount) override final;
  virtual IndexBufferHandle createResourceIndexBuffer(uint32* pIndices, const uint32 elementCount) override final;
  virtual VertexShaderHandle createVertexShader(const char* path) override final;

  virtual void cmdBindVertexBuffer(VertexBufferHandle* pVertexBufferHandle) override final;
  virtual void cmdBindIndexBuffer(IndexBufferHandle* pIndexBufferHandle) override final;
  virtual void cmdBindVertexShader(VertexShaderHandle* pVertexShaderHandle) override final;

  virtual void cmdDrawIndexed(uint32 indexCount) override final;
  virtual void excecuteCommands() override final;

  virtual void freeResource(ResourceHandle handle) override final;

private:
  void* m_hTargetWindow;      // Target Window.
  DirectX11Graphics* m_pBase;  
  static CommandBuffer cmdBuffer;
};

extern "C" DIRECTX11_API Dx11RHI* CreateDirect3D11Module();