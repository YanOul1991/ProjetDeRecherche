// Dx11VertexShader.h

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/OptimEngine.h"
#include "Dx11RHI.h"

class Dx11VertexShader final
{
 public:
  ~Dx11VertexShader() {
  }

  void createResources(const wchar* path) {
    ComPtr<ID3DBlob> pBlob{};
    ID3D11Device*    pDevice = Dx11RHI::getDevicePtr();

    D3DReadFileToBlob(path, &pBlob);

    pDevice->CreateVertexShader(
      pBlob->GetBufferPointer(),
      pBlob->GetBufferSize(),
      nullptr,
      &pShader);

    // INPUT LAYOUT
    const D3D11_INPUT_ELEMENT_DESC ied[] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    pDevice->CreateInputLayout(
      ied,
      (sizeof(ied) / sizeof(*ied)),
      pBlob->GetBufferPointer(),
      pBlob->GetBufferSize(),
      &pInput);
  }

  void bindResource() {
    ID3D11DeviceContext* pContext = Dx11RHI::getContextPtr();

    pContext->IASetInputLayout(pInput.Get());
    pContext->VSSetShader(pShader.Get(), nullptr, 0);
  }

  ComPtr<ID3D11VertexShader> pShader{};
  ComPtr<ID3D11InputLayout>  pInput{};
};