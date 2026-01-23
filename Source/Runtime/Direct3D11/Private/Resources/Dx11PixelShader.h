
// Dx11PixelShader.h

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Graphics/Resource/IPixelShader.h"
#include "IDirect3D11.h"

class Dx11PixelShader final : public IPixelShader
{
public:
	~Dx11PixelShader() override final 
	{}

	void createResources(const wchar* path) override final {
		ComPtr<ID3DBlob> pBlob;

		D3DReadFileToBlob(path, &pBlob);

		IDirect3D11::getDevicePtr()->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pShader
		);

		//printf("Pixel Shader Resources initalized.\n");
	}

	void bindResource() override final {
		IDirect3D11::getContextPtr()->PSSetShader(pShader.Get(), nullptr, 0);
	}

	ComPtr<ID3D11PixelShader> pShader;
};