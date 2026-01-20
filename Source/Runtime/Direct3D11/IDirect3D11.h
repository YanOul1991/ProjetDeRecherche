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
#include "Core/Graphics/IGraphicsModule.h"

class DirectX11Graphics;

class IDirect3D11 final : public IGraphicsModule 
{
public:
  static ID3D11Device* getDevicePtr();
  static ID3D11DeviceContext* getContextPtr();

  IDirect3D11();
  ~IDirect3D11() override final;
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

  /*----------------- TEST FIELD -----------------*/

  SGraphicResourceHandle getPixelShader(const wchar* path) override final;

  void setDrawCommand(DrawCommand& drawCommand) override final;

private:
  void* m_hTargetWindow;      // Target Window.
  DirectX11Graphics* m_pBase;  
};

extern "C" DIRECTX11_API IDirect3D11* CreateDirect3D11Module();
