#pragma once

/*
#include "Core/OptimEngine.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Direct3D11/Dx11RHI.h"
class Dx11Sampler final : public ISampler
{
public:
	~Dx11Sampler() override final {}

	void createResource() {
		D3D11_SAMPLER_DESC samplerDesc{};

    samplerDesc.Filter    = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU  = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV  = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW  = D3D11_TEXTURE_ADDRESS_WRAP;

    HRESULT hr{S_OK};
    OPTIM_TRY_DX(Dx11RHI::getDevicePtr()->CreateSamplerState(&samplerDesc, &pSampler));

		//printf("[DIRECTX 11 - Dx11Sampler] Sampler Resource created.\n");
	}

	void bindResource() override final {
		Dx11RHI::getContextPtr()->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}

	ComPtr<ID3D11SamplerState> pSampler{};
};
*/