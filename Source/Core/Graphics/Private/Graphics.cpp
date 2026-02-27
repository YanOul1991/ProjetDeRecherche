#include "Core/Graphics/Graphics.h"

#include "Core/System/FileStream.h"
#include "Core/System/SystemWindow.h"
#include "ThirdParty/SDL3/SDL.h"

static TextureResourceHandle defaultColorTexture;

// Alias for RHI creation function
using pRHIProc = IGraphicsRHI* (*)();

IGraphicsRHI* Graphics::s_pActiveGraphicsRHI{ nullptr };

IGraphicsRHI* Graphics::RHI() {
  return s_pActiveGraphicsRHI;
}

TextureResourceHandle Graphics::GetDefaultTexture() {
  return defaultColorTexture;
}

void Graphics::initalize() {
  try {
    SDL_SharedObject* handle = SDL_LoadObject("bin/directx11_ri.dll");

    pRHIProc pFactoryGraphicsModule = (pRHIProc)SDL_LoadFunction(handle, "CreateDirect3D11Module");

    s_pActiveGraphicsRHI = pFactoryGraphicsModule();

    s_pActiveGraphicsRHI->Initialize(SystemWindow::getSystemPointer());

    // Load default flat texture;
    Image defaultImage;

    FileStream::readPngImage("images/flat.png", defaultImage);
    defaultColorTexture = s_pActiveGraphicsRHI->createTextureResource(&defaultImage);
    printf("Default texture image loaded.\n");
  }
  catch (...) {
    printf("Fatal Error - Could not load graphics RHI module.\n");
    exit(-1);
  }
}