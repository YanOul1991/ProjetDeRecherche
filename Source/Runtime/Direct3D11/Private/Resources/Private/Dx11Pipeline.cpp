/**
 * Dx11Pipeline.cpp
 *
 * Par:
 *  Yanis Oulmane
 *
 * Shader Reflection
 *
 * [D3D11_SIGNATURE_PARAMETER_DESC]
 * https://learn.microsoft.com/en-us/windows/win32/api/d3d11shader/ns-d3d11shader-d3d11_signature_parameter_desc
 *
 * [D3D11_SHADER_INPUT_BIND_DESC]
 * https://learn.microsoft.com/en-us/windows/win32/api/d3d11shader/ns-d3d11shader-d3d11_shader_input_bind_desc
 *
 */

#include "../Dx11Pipeline.h"

#include "Runtime/Direct3D11/Dx11RHI.h"

#include <clocale>
#include <cstdlib>
#include <cwchar>
#include <iostream>
#include <unordered_map>
#include <vector>

static const std::unordered_map<std::string, DXGI_FORMAT> _semanticsFormat = {
  { "POSITION",    DXGI_FORMAT_R32G32B32_FLOAT },
  { "TEXCOORD",       DXGI_FORMAT_R32G32_FLOAT },
  {   "NORMAL",    DXGI_FORMAT_R32G32B32_FLOAT },
  {    "COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT },
};

static const std::unordered_map<std::string, EInputUsageSlot> _sematicsUsage = {
  { "POSITION",  EInputUsageSlot::position },
  { "TEXCOORD", EInputUsageSlot::textCoord },
  {   "NORMAL",    EInputUsageSlot::normal },
  {  "TANGENT",   EInputUsageSlot::tangent },
  {    "COLOR",     EInputUsageSlot::color },
};

static EShaderBindResourceType translateShaderInputType(D3D_SHADER_INPUT_TYPE d3dShader) {
  if (d3dShader == D3D_SIT_CBUFFER) {
    return EShaderBindResourceType::CBuffer;
  }
  else if (d3dShader == D3D_SIT_TEXTURE) {
    return EShaderBindResourceType::Texture;
  }
  else if (d3dShader == D3D_SIT_SAMPLER) {
    return EShaderBindResourceType::Sampler;
  }
  else if (d3dShader == D3D_SIT_STRUCTURED) {
    return EShaderBindResourceType::StructuredBuffer;
  }
  else {
    return EShaderBindResourceType::CBuffer;
  }
}

/*
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
  _retVal.InputSlot            = param_desc.inputSlot;
  _retVal.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
  _retVal.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
  _retVal.InstanceDataStepRate = 0;

  return _retVal;
}
*/

/**
 * \brief
 * Utility function to convert shader parameters reflection data into
 * D3D11_INPUT_ELEMENT_DESC element.
 *
 * \param param_shaderParam
 * A SSHaderParameters structure object describing the semantic.
 */
static D3D11_INPUT_ELEMENT_DESC static_createInputElementDesc(const SShaderParameters& param_shaderParam) {
  D3D11_INPUT_ELEMENT_DESC retVal{};

  retVal.SemanticName         = param_shaderParam.name.value();
  retVal.SemanticIndex        = 0;
  retVal.Format               = _semanticsFormat.at(param_shaderParam.name.value());
  retVal.InputSlot            = param_shaderParam.regist;
  retVal.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
  retVal.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
  retVal.InstanceDataStepRate = 0;

  return retVal;
}

static std::wstring optim_towstr(std::string& str) {
  uint32       size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
  std::wstring wstr(size, 0);
  MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
  return wstr;
}

Dx11Pipeline::~Dx11Pipeline() {
}

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
void Dx11Pipeline::create(ID3D11Device* pDevice, const SPipelineDesc& pipelineDesc) {
  OPTIM_CHECK_WIN_COM();

  primitiveTopology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(static_cast<int32>(pipelineDesc.primitiveTopology));

  ComPtr<ID3DBlob> pBlob;

  // VERTEX SHADER INIT

  std::string  pathVertexShader = pipelineDesc.vertexShader;
  std::wstring vertexShaderPath = optim_towstr(pathVertexShader);
  D3DReadFileToBlob(vertexShaderPath.c_str(), &pBlob);

  // Initalize reflection for vertex shader
  reflectShader(pathVertexShader, pBlob, EShaderStage::Vertex);

  OPTIM_TRY_DX(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &vertexShader));

  // INPUT LAYOUT

  std::vector<D3D11_INPUT_ELEMENT_DESC> ieds;

  // std::cout << "\n\nPrinting all for shader semantics paramerter resources (input Layout): \n";

  for (auto& param : shaderParameters) {
    /*
    EInputUsageSlot semanticUsage = _sematicsUsage.at(param.name.value());
    ::printf("Name : %s\n", param.name.value());
    ::printf("  Register : %d\n", param.regist);
    ::printf("  [Usage = %d]\n", (int)semanticUsage);
    */

    ieds.push_back(static_createInputElementDesc(param));
    inputFlags |= 1 << static_cast<uint32>(_sematicsUsage.at(param.name.value()));
  }

  OPTIM_TRY_DX(pDevice->CreateInputLayout(
    ieds.data(),
    static_cast<UINT>(ieds.size()),
    pBlob->GetBufferPointer(),
    pBlob->GetBufferSize(),
    &inputLayout));

  // PIXEL SHADER CREATION

  std::string  pathPixelShader = pipelineDesc.fragmentShader;
  std::wstring pixelShaderPath = optim_towstr(pathPixelShader);
  D3DReadFileToBlob(pixelShaderPath.c_str(), &pBlob);

  // Initalize reflection for pixel shader
  reflectShader(pathPixelShader, pBlob, EShaderStage::Fragment);

  OPTIM_TRY_DX(pDevice->CreatePixelShader(
    pBlob->GetBufferPointer(),
    pBlob->GetBufferSize(),
    nullptr,
    &pixelShader));

  // SAMPLER STATE CREATION

  D3D11_SAMPLER_DESC samplerDesc{};
  samplerDesc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

  pDevice->CreateSamplerState(&samplerDesc, &samplerState);

  // RASTERIZER STATE CREATION

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

  // DEPTH STENCIL STATE CREATION

  D3D11_DEPTH_STENCIL_DESC l_dsDesc{};
  l_dsDesc.DepthEnable    = pipelineDesc.depthStencilDescription.depthTestEnabled;
  l_dsDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(static_cast<int32>(pipelineDesc.depthStencilDescription.depthWriteMask));
  l_dsDesc.DepthFunc      = static_cast<D3D11_COMPARISON_FUNC>(static_cast<int32>(pipelineDesc.depthStencilDescription.depthComparisonFunction) + 1);
  l_dsDesc.StencilEnable  = false;

  pDevice->CreateDepthStencilState(&l_dsDesc, &depthStencilState);

  std::cout << "\n\nPrinting all for shader input bind resources : \n";
  for (auto& input : shadersInputBind) {
    String::printf("Name: %s\n", input.name.value());
    std::cout << "  Stage         : " << (int)input.shaderStage << "\n";
    std::cout << "  Resource Type : " << (int)input.resType << "\n";
    std::cout << "  Input slot    : " << input.inputSlot << "\n";
  }
  printf("\n\n");
}

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
void Dx11Pipeline::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->IASetPrimitiveTopology(primitiveTopology);
  pContext->IASetInputLayout(inputLayout.Get());
  pContext->VSSetShader(vertexShader.Get(), nullptr, 0);
  pContext->PSSetShader(pixelShader.Get(), nullptr, 0);
  pContext->PSSetSamplers(0, 1, samplerState.GetAddressOf());
  pContext->RSSetState(rasterizerState.Get());
  pContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

  Dx11RHI::StaticUpdateActivePipelineInputs(inputFlags);
}

void Dx11Pipeline::CreateVertexShader(ID3D11Device* pDevice, const char* shaderPath) {
}

void Dx11Pipeline::CreatePixelShader(ID3D11Device* pDevice, const char* shaderPath) {
}

void Dx11Pipeline::CreateRasterizerState(ID3D11Device* pDevice, const SRasterizerDescription& rastDesc) {
}

void Dx11Pipeline::CreateDSV(ID3D11Device* pDevice, const SDepthStencilDescription& dsvDesc) {
}

void Dx11Pipeline::reflectShader(const std::string& shaderName, ComPtr<ID3DBlob>& byteCode, EShaderStage stage) {
  ComPtr<ID3D11ShaderReflection> shaderReflection;
  D3DReflect(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), __uuidof(ID3D11ShaderReflection), &shaderReflection);

  D3D11_SHADER_DESC desc{};
  shaderReflection->GetDesc(&desc);

  for (UINT i = 0; i < desc.InputParameters; i++) {
    D3D11_SIGNATURE_PARAMETER_DESC sigDesc{};
    shaderReflection->GetInputParameterDesc(i, &sigDesc);

    if (stage == EShaderStage::Vertex) {
      SShaderParameters shaderParam;

      shaderParam.name   = sigDesc.SemanticName;
      shaderParam.regist = sigDesc.Register;

      shaderParameters.push_back(shaderParam);
    }

    /*
     std::cout << "  Sematic name:      " << sigDesc.SemanticName << "\n";
     std::cout << "  Sematic index:     " << sigDesc.SemanticIndex << "\n";
     std::cout << "  Sematic Register:  " << sigDesc.Register << "\n";
     std::cout << "  System Value type: " << sigDesc.SystemValueType << "\n";
     std::cout << "  Component type:    " << sigDesc.ComponentType << "\n";
    */
  }

  // std::cout << "Shader Bound Resources: " << desc.BoundResources << "\n";
  for (UINT i = 0; i < desc.BoundResources; i++) {

    D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
    shaderReflection->GetResourceBindingDesc(i, &bindDesc);

    /*
    std::cout << "  Name       : " << bindDesc.Name << "\n";
    std::cout << "  Type       : " << bindDesc.Type << "\n";
    std::cout << "  BindPoint  : " << bindDesc.BindPoint << "\n";
    std::cout << "  BindCount  : " << bindDesc.BindCount << "\n";
    std::cout << "  uFlags     : " << bindDesc.uFlags << "\n";
    std::cout << "  ReturnType : " << bindDesc.ReturnType << "\n";
    std::cout << "  Dimension  : " << bindDesc.Dimension << "\n";
    std::cout << "  NumSamples : " << bindDesc.NumSamples << "\n";
    */
    SShaderBindResource _outShaderBind;

    _outShaderBind.name        = bindDesc.Name;
    _outShaderBind.shaderStage = stage;
    _outShaderBind.resType     = translateShaderInputType(bindDesc.Type);
    _outShaderBind.inputSlot   = bindDesc.BindPoint;

    shadersInputBind.push_back(_outShaderBind);
  }
}
