/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + Application.cpp :
          Definitions for Application.h

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#pragma once

// Optim Engine libraries
#include "Core/Rendering/IRenderingModule.h"
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
    if (!m_pWindow->create(this, TEXT("Optim Engine"), 0, 0, 500, 500, nullptr)) return;
    m_pWindow->display();

    // Load Direct3d11 runtime module

    HMODULE hmod = LoadLibraryW(TEXT("bin/directx11_ri.dll"));

    if (hmod == nullptr) THROW_EXCEPTION(TEXT("Could not load the module at \"bin/directx11_ri.dll\""));

    IRenderingModule* (*pProc)() = (IRenderingModule* (*)())GetProcAddress(hmod, "CreateDirect3D11Module");

    if (pProc)
    {
      m_pRenderModule = pProc();
      m_pRenderModule->Initialize(m_pWindow->getHandle());
    }
    m_shouldRun = true;
  }
  catch (const op::Exception& e)
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

    m_pWindow->windowLoop();
    m_pRenderModule->draw();

    String winText = TEXT("Optim Engine - DirectX11");
    //winText += m_runtime;

    SetWindowTextW(reinterpret_cast<HWND>(m_pWindow->getHandle()), winText.value());

    __now         = op::time::nowHighFreq();
    __deltaTime   = (__now - __last) * (1000.0f / (float)op::time::getMachineFrequency()) / 1000.0f;
    __last        = __now;
    m_runtime     += __deltaTime;
  }
  catch (const op::Exception& e)
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
  // Free resources
  delete(m_pRenderModule);
  delete(m_pWindow);
  delete(m_pInput);
}