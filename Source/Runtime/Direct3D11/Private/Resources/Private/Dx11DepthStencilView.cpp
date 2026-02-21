#include "../Dx11DepthStencilView.h"

Dx11DepthStencilViewTexture::~Dx11DepthStencilViewTexture() {
}

void Dx11DepthStencilViewTexture::create(ID3D11Device* pDevice) {
  OPTIM_CHECK_WIN_COM();

  D3D11_TEXTURE2D_DESC depthDesc{};
  depthDesc.Width              = 1920;
  depthDesc.Height             = 1080;
  depthDesc.MipLevels          = 1;
  depthDesc.ArraySize          = 1;
  depthDesc.Format             = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthDesc.Usage              = D3D11_USAGE_DEFAULT;
  depthDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
  depthDesc.SampleDesc.Count   = 1;
  depthDesc.SampleDesc.Quality = 0;

  OPTIM_TRY_DX(pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

  printf("[Dx11DSView] Depth Stencil texture resource created\n");

  // Depth stencil view creation
  D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

  dsvDesc.Format             = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
  dsvDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
  dsvDesc.Texture2D.MipSlice = 0;

  OPTIM_TRY_DX(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvDesc, &pDepthStencilView));

  printf("[Dx11DSView] Depth Stencil view resource created\n");
}

void Dx11DepthStencilViewTexture::bind(ID3D11DeviceContext* pContext, ID3D11RenderTargetView** ppRenderTargetView) {
  pContext->OMSetRenderTargets(1, ppRenderTargetView, pDepthStencilView.Get());
}

void Dx11DepthStencilViewTexture::resize(ID3D11Device* pDevice, uint32 newWidth, uint32 newHeight) {
  OPTIM_CHECK_WIN_COM();

  D3D11_TEXTURE2D_DESC          l_depthDesc{};
  D3D11_DEPTH_STENCIL_VIEW_DESC l_dsvDesc{};

  pDepthStencil->GetDesc(&l_depthDesc);
  pDepthStencilView->GetDesc(&l_dsvDesc);

  l_depthDesc.Width  = newWidth;
  l_depthDesc.Height = newHeight;

  pDepthStencil.Reset();
  pDepthStencilView.Reset();

  OPTIM_WIN_THROW_ON_FAILED(pDevice->CreateTexture2D(&l_depthDesc, nullptr, &pDepthStencil));
  OPTIM_WIN_THROW_ON_FAILED(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &l_dsvDesc, &pDepthStencilView));
}

void Dx11DepthStencilViewTexture::clearDepthStencilView(ID3D11DeviceContext* pContext) const {
  pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
