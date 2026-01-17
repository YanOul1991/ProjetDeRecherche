/* ======================================================================================
 *  DirectX11VertexShader.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Private/Resources/IDirectX11Resource.h"

/*
 * @brief
 * A DirectX11 vertex shader data interface.
*/
class DirectX11VertexShader final : IDirectX11Resource 
{
public:
	inline DirectX11VertexShader() = default;
	inline ~DirectX11VertexShader() override 
	{ }

	inline void init(ID3D11Device* pDevice) override { 
		ComPtr<ID3DBlob> pBlob{};

		D3DReadFileToBlob(path, &pBlob);

		pDevice->CreateVertexShader(
			pBlob->GetBufferPointer(), 
			pBlob->GetBufferSize(),
			nullptr,
			&pShader
		);

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
			&pInputLayout
		);
	}

	inline void bind(ID3D11DeviceContext* pContext) override { 
		pContext->IASetInputLayout(pInputLayout.Get());
		pContext->VSSetShader(pShader.Get(), nullptr, 0);
	}

	const wchar* path{};
	ComPtr<ID3D11VertexShader>	pShader{};
	ComPtr<ID3D11InputLayout>		pInputLayout{};
};