#pragma once

// Optim Engine libraries
#include "Core/System/Application.h"
#include "Core/Rendering/IRenderingModule.h"
#include "Core/System/IWindow.h"
#include "Core/Input/Input.h"
#include "Core/Time/Time.h"
// Standard libraries
#include <iostream>
#include <memory>
#include <sstream>

extern "C" {
  EXPORT Application* CreateApplicationProc()
  {
    //std::unique_ptr<Application> application = std::make_unique<Application>();
    return new Application;
  }
}

Application::Application() :
  m_shouldRun{ false },
  m_pDirect2dModule{ nullptr },
  m_pGraphicsRenderingModule{ nullptr },
  m_pInput{ nullptr },
  m_pWindow{ nullptr }
{ }

Application::~Application() {}

bool Application::ShouldRun() const { return m_shouldRun; }

void Application::Quit() 
{ 
  MessageBoxW(0, STRING("Message quit"), STRING("Debug"), MB_OK);
  m_shouldRun = false;
}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart()
{
  std::cout << "The application has started.\n";
  // Start by assuming failure
  m_shouldRun = false;

  // Allocate resources to create a new system managed Window
  m_pWindow = new IWindow;
  if (!m_pWindow) 
    return;
  if (!m_pWindow->create(this, STRING("Optim Engine"), 0, 0, 500, 500, nullptr)) 
    return;
  m_pWindow->display();

  // Load Direct3d11 runtime module
  IRenderingModule*(*pProc)() = op::system::LOAD_LIB_PROC<PROC_PTR(IRenderingModule)>("D3D11OEI.dll", "CreateDirect3D11Module");

  if (pProc)
  {
    m_pGraphicsRenderingModule = pProc();
    m_pGraphicsRenderingModule->Initialize(m_pWindow->getHandle());
  }

  // Regitser Inputs
  m_pInput = new op::SInput;

  if (!m_pInput->initialize(reinterpret_cast<void*>(this), m_pWindow->getHandle()))
    return;

  m_shouldRun = true;
}

void Application::ApplicationLoop()
{
  /*
    * Application Loop Logic
    *   - Check if application should still be running.
    *   - Call system window's loop function for OS specifics
    *   - Draw Call
  */

  if (!m_shouldRun) 
    return;
  m_pWindow->windowLoop();
  m_pGraphicsRenderingModule->Draw();

  std::stringstream ss;

  ss << "Optim Engine | Delta Time: " << Time::getDeltaTime(ETimeUnits::sec) << "fps";
  SetWindowTextA(reinterpret_cast<HWND>(m_pWindow->getHandle()), ss.str().c_str());
}

void Application::ApplicationQuit()
{ 
  MessageBoxW(0, STRING("Destroying Application and resources"), STRING("Debug"), MB_OK);
  // Free resources
  delete(m_pDirect2dModule);
  delete(m_pGraphicsRenderingModule);
  delete(m_pWindow);
  delete(m_pInput);
}