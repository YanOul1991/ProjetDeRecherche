#include "ThirdParty/SDL3/SDL.h"
#include "Core/System/SystemWindow.h"
#include "Core/Graphics/Graphics.h"

IGraphicsRHI* Graphics::s_pActiveGraphicsRHI{nullptr};

IGraphicsRHI* Graphics::RHI()
{
	return s_pActiveGraphicsRHI;
}

void Graphics::initalize()
{

	try {
		SDL_SharedObject* handle = SDL_LoadObject("bin/directx11_ri.dll");
		IGraphicsRHI* (*pFactoryGraphicsModule)() = (IGraphicsRHI* (*)())SDL_LoadFunction(handle, "CreateDirect3D11Module");

		s_pActiveGraphicsRHI = pFactoryGraphicsModule();

		//Graphics::initalize(s_pActiveGraphicsRHI);
    s_pActiveGraphicsRHI->Initialize(SystemWindow::getSystemPointer());
	}
	catch (...) {
		printf("Fatal Error - Could not load graphics RHI module.\n");
		exit(-1);
	}
}