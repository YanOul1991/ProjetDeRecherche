#pragma once

#include "Application.h"

// If platform is Windows
#if defined(_WIN32) || defined(_WIN64)
// Define WIN_API to use Windows specific APIs
// and include Windows specific headers

#define WIN_API
#include "Global/WindowsSettings.h"
#include "DirectX/Direct2D/Direct2D.h"
typedef Direct2DRenderer* (*CreateD2DRendererFunc)();
#endif

Application::Application() :
  loops{ 0 },
  isRunning{ false }
{}

Application::~Application() {}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart()
{
#if defined(WIN_API)
  MessageBoxA(0, "Application Class will start with Windows API", "Application Class Debug", MB_OK);
  isRunning = true;
  HMODULE moduleD2d = LoadLibraryA("Direct2DOEI.dll");

  if (!moduleD2d)
  {
    MessageBoxA(0, "Failed to Load Direct2D module", "Application Class Debug", MB_OK + MB_ICONEXCLAMATION);
  }

  if (moduleD2d)
  {
    CreateD2DRendererFunc createFunc = (CreateD2DRendererFunc)GetProcAddress(moduleD2d, "CreateDirect2DRenderer");
    if (createFunc)
    {
      MessageBoxA(
        0 ,
        "Sucessfully references Direct2D.dll module's CreateDirect2DRenderer Function", 
        "Application Class Debug", 
        MB_OK + MB_ICONASTERISK
      );

      Direct2DRenderer* renderer = createFunc();
    }
    else
    {

      MessageBoxA(
        0 ,
        "Failed to reference Direct2D.dll module's CreateDirect2DRenderer Function", 
        "Application Class Debug", 
        MB_OK + MB_ICONASTERISK
      );
    }
  }
#endif
}

void Application::ApplicationLoop()
{
#if defined(WIN_API)
  while (isRunning)
  {
    MessageBoxA(0, "Application Loop!", "Application Class Debug", MB_OK);
    loops++;
    isRunning = loops >= 5;
  }
#endif
}

void Application::ApplicationQuit()
{
#if defined(WIN_API)
  MessageBoxA(0, "Application Quit", "Applicatio Class Debug", MB_OK);
#endif
}