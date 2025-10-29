#pragma once

#include "Application.h"
#include "Editor/EditorSystemWindow.h"

#if defined(WINDOWS_OS)
#include "DirectX/Direct2D/Direct2D.h"

typedef IRenderingModule* (*CreateD2DRendererFunc)();
#endif

Application::Application() :
  m_shouldRun{ false },
  m_pEditorOsWindow { nullptr },
  m_pDirect2dModule { nullptr },
  m_pInput { nullptr }
{ }

Application::~Application() {}

bool Application::ShouldRun() const { return m_shouldRun; }
void Application::Quit() { m_shouldRun = false; }

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart()
{
  std::cout << "The application has started.\n";

  // Start by assuming failure
  m_shouldRun = false;

  // Create the Main Window
  m_pEditorOsWindow = EditorSystemWindow::CreateEditorSystemWindow(this);

  if (!m_pEditorOsWindow)
  {
    std::cout << "Application has failed to create the EditorSystemWindow,\n";
    return;
  }

  if (!m_pEditorOsWindow->Create(L"Optim Engine Editor", 0, 0, 500, 500, nullptr))
  {
    std::cout << "Failed to create the main application window.\n";
    return;
  }

  // Display the main Window
  m_pEditorOsWindow->Display();

/*
  Windows platform instructions
*/
#if defined(WINDOWS_OS)
  HMODULE hmodDirect2d = LoadLibraryA("Direct2DOEI.dll");
  if (hmodDirect2d)
  {
    CreateD2DRendererFunc pProcCreateD2DRenderer = (CreateD2DRendererFunc)GetProcAddress(hmodDirect2d, "CreateDirect2DRenderer");
    if (pProcCreateD2DRenderer)
    {
      m_pDirect2dModule = pProcCreateD2DRenderer();
      m_pDirect2dModule->SetContext(m_pEditorOsWindow->m_pHandle);
    }
  }
#endif

  m_pInput = Input::Initalize(this, m_pEditorOsWindow->GetHandle());

  m_shouldRun = true;
}

void Application::ApplicationLoop()
{
  if (!m_shouldRun) return;
#if defined(WINDOWS_OS)
  while (PeekMessageW(&m_windowsMsg, 0, 0, 0, PM_REMOVE) > 0)
  {
    TranslateMessage(&m_windowsMsg);
    DispatchMessageW(&m_windowsMsg);
  }
#endif
  m_pDirect2dModule->Draw();
}

void Application::ApplicationQuit()
{ }