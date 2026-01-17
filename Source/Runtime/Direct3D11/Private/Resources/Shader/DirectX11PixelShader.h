/* ======================================================================================
 *  DirectX11PixelShader.h:
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
 * A DirectX11 pixel shader data interface.
*/
class DirectX11PixelShader final : IDirectX11Resource
{
public:
	inline DirectX11PixelShader() = default;
	inline ~DirectX11PixelShader() override 
	{ }

	inline void init(ID3D11Device* pDevice) override {
		ComPtr<ID3DBlob> pBlob;

		D3DReadFileToBlob(path, &pBlob);

		pDevice->CreatePixelShader(
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			nullptr,
			&pShader
		);
	}

	inline void bind(ID3D11DeviceContext* pContext) override {
		pContext->PSSetShader(pShader.Get(), nullptr, 0);
	}

	const wchar* path{};
	ComPtr<ID3D11PixelShader> pShader;
};