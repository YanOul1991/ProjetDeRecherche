/**
 * Dx11Pipeline.h
 *
 * Par:
 *  Yanis Oulmane.
 */

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Graphics/Resource/GraphicPipeline.h"
#include "Private/Resources/IDx11Resource.h"

#include <string>
#include <vector>


/**
 * \brief
 * The Dx11Pipeline class object serves to define how to render objects
 * and defined reflection for HLSL shaders, to allow dynamic
 * parameters and input binding.
 */
class Dx11Pipeline final : public IDx11Resource
{
 public:
  Dx11Pipeline() = default;

  virtual ~Dx11Pipeline() override final;

  /**
   * \brief
   * Crate a pipeline resource object.
   *
   * \param pDevice
   * A pointer to a ID3D11Device
   *
   * \param pipelineDesc
   * A SPipelineDesc object.
   */
  void create(ID3D11Device* pDevice, const SPipelineDesc& pipelineDesc);

  /**
   * \brief
   * Bind the Dx11Pipeline.
   *
   * \param pContext
   * Pointer to a ID3D11DeviceContext object.
   *
   * \param ppRenderTargetView
   * A pointer to a pointer to ID3D11RenderTargetView
   */
  virtual void bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) override final;

  uint32 inputFlags = 0;

  D3D_PRIMITIVE_TOPOLOGY          primitiveTopology{}; // [x] - [x]
  ComPtr<ID3D11InputLayout>       inputLayout{};       // [x] - [x]
  ComPtr<ID3D11VertexShader>      vertexShader{};      // [x] - [x]
  ComPtr<ID3D11PixelShader>       pixelShader;         // [x] - [x]
  ComPtr<ID3D11DepthStencilState> depthStencilState{}; // [x] - [x]
  ComPtr<ID3D11RasterizerState>   rasterizerState{};   // [x] - [x]
  ComPtr<ID3D11SamplerState>      samplerState{};      // [x] - [x]

  ComPtr<ID3D11BlendState>     blendState{};           // NOT IMPLEMENTED
  ComPtr<ID3D11GeometryShader> geometryShader{};       // NOT IMPLEMENTED

  std::vector<SShaderParameters>   shaderParameters{};
  std::vector<SShaderBindResource> shadersInputBind{};

 private:
  void reflectShader(const std::string& shaderName, ComPtr<ID3DBlob>& byteCode, EShaderStage stage);
};