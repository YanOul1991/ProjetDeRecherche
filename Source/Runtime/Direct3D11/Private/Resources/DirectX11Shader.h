/* ======================================================================================
 *  DirectX11Shader.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include <iostream>

struct VertexShader
{
	const wchar* path;
	ComPtr<ID3D11VertexShader> pShader;
	ComPtr<ID3D11InputLayout>  pInputLayout;
};

struct PixelShader
{
	const wchar* path;
	ComPtr<ID3D11PixelShader> pShader;
};

class GFXMaterial
{
public:
	inline GFXMaterial() = default;
	inline ~GFXMaterial() = default;

	inline void setPath(const wchar* vertexShaderPath, const wchar* pixelShaderPath)
	{
		vertexShader.path = vertexShaderPath;
		pixelShader.path	= pixelShaderPath;
	}

	inline void loadShaders(ID3D11Device* device)
	{
		ComPtr<ID3DBlob> pBlob;

		D3DReadFileToBlob(pixelShader.path, &pBlob);

		device->CreatePixelShader(
			pBlob->GetBufferPointer(), 
			pBlob->GetBufferSize(),
			nullptr,
			&pixelShader.pShader
		);

		D3DReadFileToBlob(vertexShader.path, &pBlob);

		device->CreateVertexShader(
			pBlob->GetBufferPointer(), 
			pBlob->GetBufferSize(),
			nullptr,
			&vertexShader.pShader
		);

		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"Position" , 0,  DXGI_FORMAT_R32G32B32_FLOAT,  0, 0,   D3D11_INPUT_PER_VERTEX_DATA,  0 }
		};

		device->CreateInputLayout(
			ied,
			std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&vertexShader.pInputLayout
		);
	}

	VertexShader vertexShader;
	PixelShader  pixelShader;
};
