#include "../Dx11Texture.h"

Dx11TextureResource::~Dx11TextureResource() {
}

void Dx11TextureResource::create(ID3D11Device* pDevice, const Image* pImage) {
  OPTIM_WIN_COM_CHECK_START();

  D3D11_TEXTURE2D_DESC   textDesc{};
  D3D11_SUBRESOURCE_DATA subRes{};

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

  OPTIM_TRY_DX(pDevice->CreateTexture2D(&textDesc, &subRes, &pTexture));
  // printf("[Dx11TextureResource] Texture2D resource created\n");

  // ################# SHADER RESOURCE VIEW INITIALIZATION
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};

  srvDesc.Format                    = textDesc.Format;
  srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.Texture2D.MipLevels       = 1;

  OPTIM_TRY_DX(pDevice->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pResourceView));

  // printf("[Dx11TextureResource] Shader Resource View resource created\n");
}

void Dx11TextureResource::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->PSSetShaderResources(0, 1, pResourceView.GetAddressOf());
}
