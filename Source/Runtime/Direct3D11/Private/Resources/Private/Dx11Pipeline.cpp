// Dx11Pipeline.cpp

#include "../Dx11Pipeline.h"

#include <clocale>
#include <cstdlib>
#include <cwchar>
#include <iostream>
#include <vector>

static constexpr DXGI_FORMAT translateDXGIFormat(EGraphicsFormat format) {
  switch (format) {
  case EGraphicsFormat::r32g32b32a32_typeless: return DXGI_FORMAT_R32G32B32A32_TYPELESS;
  case EGraphicsFormat::r32g32b32a32_float   : return DXGI_FORMAT_R32G32B32A32_FLOAT;
  case EGraphicsFormat::r32g32b32a32_uint    : return DXGI_FORMAT_R32G32B32A32_UINT;
  case EGraphicsFormat::r32g32b32a32_sint    : return DXGI_FORMAT_R32G32B32A32_SINT;
  case EGraphicsFormat::r32g32b32_typeless   : return DXGI_FORMAT_R32G32B32_TYPELESS;
  case EGraphicsFormat::r32g32b32_float      : return DXGI_FORMAT_R32G32B32_FLOAT;
  case EGraphicsFormat::r32g32b32_uint       : return DXGI_FORMAT_R32G32B32_UINT;
  case EGraphicsFormat::r32g32b32_sint       : return DXGI_FORMAT_R32G32B32_SINT;
  case EGraphicsFormat::r32g32_typeless      : return DXGI_FORMAT_R32G32_TYPELESS;
  case EGraphicsFormat::r32g32_float         : return DXGI_FORMAT_R32G32_FLOAT;
  case EGraphicsFormat::r32g32_uint          : return DXGI_FORMAT_R32G32_UINT;
  case EGraphicsFormat::r32g32_sint          : return DXGI_FORMAT_R32G32_SINT;
  default                                    : return DXGI_FORMAT_UNKNOWN;
  }
}

static D3D11_INPUT_ELEMENT_DESC translateInput(SPipelineInputDescription param_desc) {
  D3D11_INPUT_ELEMENT_DESC _retVal{};

  _retVal.SemanticName         = param_desc.name;
  _retVal.SemanticIndex        = 0;
  _retVal.Format               = translateDXGIFormat(param_desc.format);
  _retVal.InputSlot            = 0;
  _retVal.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
  _retVal.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
  _retVal.InstanceDataStepRate = 0;

  return _retVal;
}

static std::wstring optim_towstr(std::string& str) {
  uint32       size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
  std::wstring wstr(size, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
  return wstr;
}

static void StaticCreateVertexShader(ID3D11Device* pDevice, const char* path) {
}

Dx11Pipeline::~Dx11Pipeline() {
}

void Dx11Pipeline::create(ID3D11Device* pDevice, const SPipelineDesc& pipelineDesc) {
  OPTIM_CHECK_WIN_COM();

  primitiveTopology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(static_cast<int32>(pipelineDesc.primitiveTopology));

  ComPtr<ID3DBlob> pBlob;

  // -----------------------------------------------
  // ------------------------ VERTEX SHADER CREATION
  // -----------------------------------------------

  std::string  pathVertexShader = pipelineDesc.vertexShader;
  std::wstring vertexShaderPath = optim_towstr(pathVertexShader);
  D3DReadFileToBlob(vertexShaderPath.c_str(), &pBlob);

  OPTIM_TRY_DX(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &vertexShader));

  printf("[Dx11Pipeline] Vertex Shader resource created...\n");

  // -----------------------------------------------
  // ------------------------  INPUT LAYOUT CREATION
  // -----------------------------------------------

  std::vector<D3D11_INPUT_ELEMENT_DESC> ieds;

  for (auto& input : pipelineDesc.inputs) {
    ieds.push_back(translateInput(input));
  }

   //std::vector<D3D11_INPUT_ELEMENT_DESC> ieds = {
   //  {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
   //  {"TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
   //  {  "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
   //};

  OPTIM_TRY_DX(pDevice->CreateInputLayout(
    ieds.data(),
    ieds.size(),
    pBlob->GetBufferPointer(),
    pBlob->GetBufferSize(),
    &inputLayout));

  printf("[Dx11Pipeline] Input layout resource created...\n");

  // -----------------------------------------------
  // ------------------------  PIXEL SHADER CREATION
  // -----------------------------------------------

  std::string  pathPixelShader = pipelineDesc.fragmentShader;
  std::wstring pixelShaderPath = optim_towstr(pathPixelShader);
  D3DReadFileToBlob(pixelShaderPath.c_str(), &pBlob);

  OPTIM_TRY_DX(pDevice->CreatePixelShader(
    pBlob->GetBufferPointer(),
    pBlob->GetBufferSize(),
    nullptr,
    &pixelShader));

  printf("[Dx11Pipeline] Vertex Shader resource created...\n");

  // ------------------------------------------------
  // ------------------------  SAMPLER STATE CREATION
  // ------------------------------------------------

  D3D11_SAMPLER_DESC samplerDesc{};
  samplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

  pDevice->CreateSamplerState(&samplerDesc, &samplerState);

  printf("[Dx11Pipeline] Sampler state resource created...\n");

  // -------------------------------------------------------------
  // ------------------------  RASTERIZER STATE RESOURCE  CREATION
  // -------------------------------------------------------------

  D3D11_RASTERIZER_DESC l_rastDesc{};
  l_rastDesc.FillMode              = static_cast<D3D11_FILL_MODE>(static_cast<int32>(pipelineDesc.rasterizerDescription.fillMode) + 2);
  l_rastDesc.CullMode              = static_cast<D3D11_CULL_MODE>(static_cast<int32>(pipelineDesc.rasterizerDescription.cullMode) + 1);
  l_rastDesc.FrontCounterClockwise = pipelineDesc.rasterizerDescription.faceWinding == ERasterizerFaceWinding::CounterClockWise;
  l_rastDesc.DepthBias             = pipelineDesc.rasterizerDescription.depthBias;
  l_rastDesc.DepthBiasClamp        = 0.0f;
  l_rastDesc.SlopeScaledDepthBias  = pipelineDesc.rasterizerDescription.slopeScaledDepthBias;
  l_rastDesc.DepthClipEnable       = TRUE;
  l_rastDesc.ScissorEnable         = FALSE;
  l_rastDesc.MultisampleEnable     = FALSE;
  l_rastDesc.AntialiasedLineEnable = FALSE;

  pDevice->CreateRasterizerState(&l_rastDesc, &rasterizerState);

  printf("[Dx11Pipeline] Rasterizer state resource created...\n");

  // ----------------------------------------------------------------
  // ------------------------  DEPTH STENCIL STATE RESOURCE  CREATION
  // ----------------------------------------------------------------

  D3D11_DEPTH_STENCIL_DESC l_dsDesc{};
  l_dsDesc.DepthEnable    = pipelineDesc.depthStencilDescription.depthTestEnabled;
  l_dsDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(static_cast<int32>(pipelineDesc.depthStencilDescription.depthWriteMask));
  l_dsDesc.DepthFunc      = static_cast<D3D11_COMPARISON_FUNC>(static_cast<int32>(pipelineDesc.depthStencilDescription.depthComparisonFunction) + 1);
  l_dsDesc.StencilEnable  = false;

  pDevice->CreateDepthStencilState(&l_dsDesc, &depthStencilState);

  printf("[Dx11Pipeline] Depth stencil state resource created...\n");
}

/**
 * @brief
 * Binds resources of the pipeline, param_descibing how to
 * perform rendering.
 */
void Dx11Pipeline::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->IASetPrimitiveTopology(primitiveTopology);
  pContext->IASetInputLayout(inputLayout.Get());
  pContext->VSSetShader(vertexShader.Get(), nullptr, 0);
  pContext->PSSetShader(pixelShader.Get(), nullptr, 0);
  pContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
  pContext->RSSetState(rasterizerState.Get());
  pContext->OMSetDepthStencilState(depthStencilState.Get(), 1);
}