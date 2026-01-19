
// Dx11VertexShader.h

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Graphics/Resource/IVertexShader.h"
#include "IDirect3D11.h"

class Dx11VertexShader final : public IVertexShader
{
public:
	~Dx11VertexShader() override final
	{}

	void createResources(const wchar* path) override final {
		ComPtr<ID3DBlob>	pBlob{};
		ID3D11Device* pDevice	=	IDirect3D11::getDevicePtr();

		D3DReadFileToBlob(path, &pBlob);

		pDevice->CreateVertexShader(
			pBlob->GetBufferPointer(), 
			pBlob->GetBufferSize(),
			nullptr,
			&pShader
		);

		// INPUT LAYOUT
				// Input
		const D3D11_INPUT_ELEMENT_DESC ied[] = {
			{"POSITION" , 0,  DXGI_FORMAT_R32G32B32_FLOAT,  0, 0,   D3D11_INPUT_PER_VERTEX_DATA,  0 },
			{"TEXCOORD" , 0,  DXGI_FORMAT_R32G32_FLOAT,     0, 12,  D3D11_INPUT_PER_VERTEX_DATA,  0 }
		};

		pDevice->CreateInputLayout(
			ied,
			(sizeof(ied) / sizeof(*ied)),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInput
		);

		printf("Vertex Shader Resources initalized.\n");
	}

	void bindResource() override final {
		ID3D11DeviceContext* pContext = IDirect3D11::getContextPtr();

		pContext->IASetInputLayout(pInput.Get());
		pContext->VSSetShader(pShader.Get(), nullptr, 0);
	}

	ComPtr<ID3D11VertexShader>	pShader{};
	ComPtr<ID3D11InputLayout>		pInput{};
};