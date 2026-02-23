// Dx11Pipeline.h

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Graphics/Resource/GraphicPipeline.h"
#include "Private/Resources/IDx11Resource.h"

class Dx11Pipeline final : public IDx11Resource
{
 public:
  Dx11Pipeline() = default;

  virtual ~Dx11Pipeline() override final;

  void create(ID3D11Device* pDevice, const SPipelineDesc& pipelineDesc);

  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  uint32 inputFlags = 0;

  D3D_PRIMITIVE_TOPOLOGY          primitiveTopology{}; // [x] - [x]
  ComPtr<ID3D11InputLayout>       inputLayout{};       // [x] - [x]
  ComPtr<ID3D11VertexShader>      vertexShader{};      // [x] - [x]
  ComPtr<ID3D11PixelShader>       pixelShader;         // [x] - [x]
  ComPtr<ID3D11DepthStencilState> depthStencilState{}; // [x] - [x]
  ComPtr<ID3D11RasterizerState>   rasterizerState{};   // [x] - [x]
  ComPtr<ID3D11SamplerState>      samplerState{};      // [x] - [x]

  // NOT YET IMPLEMENTED
  ComPtr<ID3D11BlendState>     blendState{};
  ComPtr<ID3D11GeometryShader> geometryShader{};
};