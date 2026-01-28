/* ======================================================================================
 *  Application.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Time/Time.h"

#include "Core/Types/Color.h"
#include "Core/Types/string.h"

#include "Core/Exception/exception.h"

#include "Core/Object/Image/Image.h"
#include "Core/Object/Object.h"

#include "Core/Utilities/Random/Random.h"
#include "Core/Utilities/Pointer/UniquePtr.h"

#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/Graphics.h"

#include "Core/System/FileStream.h"
#include "Core/System/SystemWindow.h"
#include "Core/System/ModelLoader.h"
#include "Core/System/Application.h"

//#include <iostream>
//#include <sstream>
//#include <format>
//#include <memory>
//#include <fstream>
//#include <string>
//#include "Core/Input/Input.h"

/* #########################
    LOCAL TESTING FIELDS
######################### */

static IVertexShader*       _TEST_pVertexShader{};
static IPixelShader*        _TEST_pPixelShader{};
static ITextureResource*    _TEST_pTextureResource{};
static ISampler*            _TEST_pSampler{};

static std::vector<UniquePtr<Mesh>> _list_meshes{};
static UniquePtr<SystemWindow>      g_uptrSystemWindow{};

static VertexShaderHandle _handle_vertexShader{};

extern "C" CORE_API Application* CreateApplicationProc() {
  return new Application;
}

float Application::m_runtime{ 0.0f };
float Application::m_deltaTime{ 1.0f };

Application::Application(){}
Application::~Application(){}

bool Application::ShouldRun() const 
{ 
  return m_shouldRun; 
}

float Application::getRuntime()
{
  return m_runtime;
}

float Application::getDeltaTime()
{
  return m_deltaTime;
}

void Application::Quit() 
{ 
  m_shouldRun = false; 
}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart()
{
  try {
    g_uptrSystemWindow.init();
    g_uptrSystemWindow->initialize("Optim Engine");
    Graphics::initalize();
    g_uptrSystemWindow->showWindow();

    /*
     * OLD POINTER SYSTEM FOR RESOURCE CREATION
    */

    _handle_vertexShader = Graphics::RHI()->createVertexShader("bin/PhongVertexShader.cso");
    _TEST_pPixelShader  = Graphics::RHI()->createPixelShader(TEXT("bin/PhongPixelShader.cso"));

    // Load image for texture
    Image srcImage;
    FileStream::readPngImage("images/jeff2.png", srcImage);
    _TEST_pTextureResource  = Graphics::RHI()->createTextureResource(&srcImage);

    // Create sampler resource
    _TEST_pSampler = Graphics::RHI()->createSamplerResource();
    //_hPixelShader = m_pRenderModule->getPixelShader(TEXT("bin/PixelShader.cso"));

    m_shouldRun = true;
  }
  catch (const Exception& e) {
    String fullMessage = String(e.whatDescriptive());
    MessageBoxW(0, fullMessage.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e) {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...) {
    MessageBoxW(0, TEXT("Unknown details, for exception thrown"), TEXT("Exception..."), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
}

void Application::ApplicationLoop()
{
  try {
    static uint64 __now;
    static uint64 __last = op::time::nowHighFreq();

    if (!g_uptrSystemWindow->loop()) {
      Quit();
      return;
    }

    Graphics::RHI()->bindPixelShader(_TEST_pPixelShader);
    Graphics::RHI()->bindTexture(_TEST_pTextureResource);
    Graphics::RHI()->bindSampler(_TEST_pSampler);

    Graphics::RHI()->cmdBindVertexShader(&_handle_vertexShader);

    for (int i = 0; i < _list_meshes.size(); i++) {
      Graphics::RHI()->cmdBindVertexBuffer(&_list_meshes[i]->vertexBufferHandle);
      Graphics::RHI()->cmdBindIndexBuffer(&_list_meshes[i]->indexBufferHandle);
      Graphics::RHI()->cmdDrawIndexed(_list_meshes[i]->indexCount);
    }
    Graphics::RHI()->draw();

    __now         = op::time::nowHighFreq();
    m_deltaTime   = (__now - __last) * (1000.0f / (float)op::time::getMachineFrequency()) / 1000.0f;
    __last        = __now;
    m_runtime     += m_deltaTime;
  }
  catch (const Exception& e) {
    String fullMessage = String(e.type());
    fullMessage +=  String(TEXT("\n\n[Description]\n")) + String(e.what()) + String(TEXT("\n[File]\n")) + e.getFile();

    MessageBoxW(0, fullMessage.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e) {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...) {
    MessageBoxW(0, TEXT("Unknown details, for exception thrown"), TEXT("Exception..."), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
}

void Application::ApplicationQuit()
{ 
  printf("Application quitting...\n");
}

CORE_API void OptimEditor::processFile(const char* param_cstrFilePath) 
{
  UniquePtr<Mesh> l_uptrMesh;
  l_uptrMesh.init();

  OptimEditor::loadFbxModel(*l_uptrMesh, param_cstrFilePath);

  (*l_uptrMesh).vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(l_uptrMesh->vertices, l_uptrMesh->vertexCount);
  (*l_uptrMesh).indexBufferHandle = Graphics::RHI()->createResourceIndexBuffer(l_uptrMesh->indices, l_uptrMesh->indexCount);

  _list_meshes.push_back(l_uptrMesh.move());
}
