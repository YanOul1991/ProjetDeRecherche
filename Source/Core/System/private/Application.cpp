/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + Application.cpp :
          Definitions for Application.h

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

// Optim Engine libraries
#include "Core/Graphics/IGraphicsModule.h"
#include "Core/System/IWindow.h"
#include "Core/Input/Input.h"
#include "Core/Time/Time.h"
#include "Core/Color/Color.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"

#include "Core/System/Application.h"

#include <format>

// Standard libraries
#include <iostream>
#include <memory>
#include <sstream>

extern "C" {
  CORE_API Application* CreateApplicationProc()
  {
    //std::unique_ptr<Application> application = std::make_unique<Application>();
    return new Application;
  }
}

float Application::m_runtime{ 0.0f };

Application::Application() :
  m_shouldRun     { false },
  m_pRenderModule { nullptr },
  m_pInput        { nullptr },
  m_pWindow       { nullptr }
{ }

Application::~Application() {}

bool Application::ShouldRun() const { return m_shouldRun; }

float Application::getRuntime()
{
  return m_runtime;
}

void Application::Quit() 
{ 
  MessageBox(0, TEXT("Quit function called"), TEXT("Debug"), MB_OK);
  m_shouldRun = false;
}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart()
{
  try
  {
    // Start by assuming failure
    m_shouldRun = false;

    // Allocate resources to create a new system managed Window
    m_pWindow = new IWindow;
    if (!m_pWindow) return;
    if (!m_pWindow->create(this, TEXT("Optim Engine | <DirectX11>"), 0, 0, 500, 500, nullptr)) return;

    // Load Direct3d11 runtime module
    HMODULE hmod = LoadLibraryW(TEXT("bin/directx11_ri.dll"));

    if (hmod == nullptr) THROW_EXCEPTION(TEXT("Could not load the module at \"bin/directx11_ri.dll\""));

    IGraphicsModule* (*pProc)() = (IGraphicsModule* (*)())GetProcAddress(hmod, "CreateDirect3D11Module");

    if (pProc)
    {
      m_pRenderModule = pProc();
      m_pRenderModule->Initialize(m_pWindow->getHandle());
    }
    m_shouldRun = true;
  }
  catch (const Exception& e)
  {
    String fullMessage = String(e.whatDescriptive());
    MessageBoxW(0, fullMessage.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e)
  {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...)
  {
    MessageBoxW(0, TEXT("Unknown details, for exception thrown"), TEXT("Exception..."), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
}

void Application::ApplicationLoop()
{
  try
  {
    static uint64 __now;
    static uint64 __last = op::time::nowHighFreq();
    static float __deltaTime{ 1.0f };

    if (!m_shouldRun) return;

    if (m_pWindow->windowLoop())
    {
      MessageBox(0, TEXT("Quit message for windows recieved"), TEXT("Debug"), MB_OK);
      Quit();
      return;
    }

    m_pRenderModule->draw();

    String winText = TEXT("Optim Engine <DirectX11>");

    //float ndcX = ((float)Mouse::posX / 1920) * 2 - 1.0f;
    //float ndcY = -((float)Mouse::posY / 1080) * 2 + 1.0f;

    //float testX = ((1920.0 / 2 - 100.0f) / 1920) * 2 - 1.0f;

    //winText += String(TEXT(" | ")) + String(testX) + String(TEXT(", ")) + String(Mouse::posY);

    winText += String(TEXT(" ")) + String(static_cast<int>(1.0f / __deltaTime)) + TEXT("fps");

    SetWindowTextW(reinterpret_cast<HWND>(m_pWindow->getHandle()), winText.value());

    __now         = op::time::nowHighFreq();
    __deltaTime   = (__now - __last) * (1000.0f / (float)op::time::getMachineFrequency()) / 1000.0f;
    __last        = __now;
    m_runtime     += __deltaTime;
  }
  catch (const Exception& e)
  {
    String fullMessage = String(e.type());
    fullMessage 
      +=  String(TEXT("\n\n[Description]\n")) + String(e.what())
      +   String(TEXT("\n[File]\n")) + e.getFile();

    MessageBoxW(0, fullMessage.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e)
  {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...)
  {
    MessageBoxW(0, TEXT("Unknown details, for exception thrown"), TEXT("Exception..."), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
}

void Application::ApplicationQuit()
{ 
  MessageBox(0, TEXT("ApplicationQuit Function called"), TEXT("Debug"), MB_OK);
  // Free resources
  delete(m_pRenderModule);
  delete(m_pWindow);
  delete(m_pInput);
}