
// Dx11PixelShader.h

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Dx11RHI.h"

class Dx11PixelShader final
{
public:
	~Dx11PixelShader() 
	{}

	void createResources(const wchar* path) {
		ComPtr<ID3DBlob> pBlob;

		D3DReadFileToBlob(path, &pBlob);

		Dx11RHI::getDevicePtr()->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pShader
		);
	}

	void bindResource() {
		Dx11RHI::getContextPtr()->PSSetShader(pShader.Get(), nullptr, 0);
	}

	ComPtr<ID3D11PixelShader> pShader;
};