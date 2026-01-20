#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Graphics/Resource/ITextureResource.h"
#include "IDirect3D11.h"

class Dx11TextureResource final : public ITextureResource 
{
public:
	~Dx11TextureResource() override final {}

	void createResource(const Image* pImage) override final {
    HRESULT hr = S_OK;

    // ################# TEXTURE RESOURCE INITIALIZATION
		D3D11_TEXTURE2D_DESC		textDesc{};
    D3D11_SUBRESOURCE_DATA  subRes{};

		textDesc.Width              = pImage->width;
    textDesc.Height             = pImage->height;
    textDesc.MipLevels          = 1;
    textDesc.ArraySize          = 1;
    textDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    textDesc.SampleDesc.Count   = 1;
    textDesc.SampleDesc.Quality = 0;
    textDesc.Usage              = D3D11_USAGE_DEFAULT;
    textDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    textDesc.CPUAccessFlags     = 0;
    textDesc.MiscFlags          = 0;

    subRes.pSysMem          = pImage->pixels;
    subRes.SysMemPitch      = pImage->width * sizeof(op::color::SColor);
    subRes.SysMemSlicePitch = 0;

    OPTIM_TRY_DX(IDirect3D11::getDevicePtr()->CreateTexture2D(&textDesc, &subRes, &pTexture));
    printf("Texture resource created.\n");

    // ################# SHADER RESOURCE VIEW INITIALIZATION
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

    srvDesc.Format                    = textDesc.Format;
    srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels       = 1;

    OPTIM_TRY_DX(IDirect3D11::getDevicePtr()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pResourceView));
    printf("Shader resource view created.\n");
	}

	void bindResource() override final {
    IDirect3D11::getContextPtr()->PSSetShaderResources(0, 1, pResourceView.GetAddressOf());
	}

	ComPtr<ID3D11Texture2D> pTexture{};
	ComPtr<ID3D11ShaderResourceView> pResourceView{};
};