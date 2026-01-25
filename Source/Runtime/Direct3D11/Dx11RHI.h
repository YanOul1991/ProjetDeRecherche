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

class DirectX11Graphics;

enum ECommandType {
  bindPipeline,
  bindVertexBuffer,
  bindIndexBuffer,
  bindTexture,
  drawIndexed
};

struct SCommand {
  ECommandType type;
  uint32 dataOffset;
};

struct SCommandBuffer {
  void push(ECommandType param_cmdType, uint8* param_pData, uint32 param_dataSize) {
    SCommand cmd{};
    cmd.type = param_cmdType;
    cmd.dataOffset = data.size();
    commands.push_back(cmd);
    data.insert(data.end(), param_pData, param_pData + param_dataSize);
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

  // NEW VIRTUAL FUNCTIONS TO TEST

  IVertexBuffer*    createVertexBuffer(Vertex* pVertices, const uint32& bufferElementCount) override final;
  IIndexBuffer*     createIndexBuffer(uint32* pIndices, const uint32& bufferElementCount) override final;
  IVertexShader*    createVertexShader(const wchar* path) override final;
  IPixelShader*     createPixelShader(const wchar* path) override final;
  ITextureResource* createTextureResource(const Image* pImage) override final;
  ISampler*         createSamplerResource() override final;

  void bindVertexBuffer(IVertexBuffer* pVertexBuffer) override final;
  void bindIndexBuffer(IIndexBuffer* pIndexBuffer) override final;
  void bindVertexShader(IVertexShader* pVertexShader) override final;
  void bindPixelShader(IPixelShader* pPixelShader) override final;
  void bindTexture(ITextureResource* pTexture) override final;
  void bindSampler(ISampler* pSampler) override final;

  inline void cmdDrawIndexed(uint32 indexCount) override final {
    cmdBuffer.push(ECommandType::drawIndexed, (uint8*)(&indexCount), sizeof(uint32));
  }

  inline void excecuteCommands() override final;

  /*----------------- TEST FIELD -----------------*/

  SGraphicResourceHandle getPixelShader(const wchar* path) override final;

  void setDrawCommand(DrawCommand& drawCommand) override final;
  void setCommandBuffer(DrawCommand* pDrawCommandBuffer, uint32 count) override final;

private:
  void* m_hTargetWindow;      // Target Window.
  DirectX11Graphics* m_pBase;  
  static SCommandBuffer cmdBuffer;
};

extern "C" DIRECTX11_API Dx11RHI* CreateDirect3D11Module();