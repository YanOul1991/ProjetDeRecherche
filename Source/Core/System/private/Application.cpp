/* ======================================================================================
 *  Application.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/Input/Input.h"
#include "Core/Time/Time.h"
#include "Core/Types/Color.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/System/FileStream.h"
#include "Core/System/SystemWindow.h"
#include "Core/Object/Image/Image.h"
#include "Core/Object/Object.h"

#include "Core/Utilities/Random/Random.h"
#include "Core/Utilities/Pointer/UniquePtr.h"

#include "Core/System/ModelLoader.h"

#include "ThirdParty/SDL3/SDL.h"

#include <iostream>
#include <sstream>
#include <format>
#include <memory>
#include <fstream>
#include <string>

#include "Core/Graphics/Mesh.h"
//#include "Core/Graphics/Graphics.h"
//#include "Core/Graphics/Resource/IVertexShader.h"
//#include "Core/Graphics/Resource/IPixelShader.h"
//#include "Core/Graphics/Resource/ITextureResource.h"
//#include "Core/Graphics/Resource/ISampler.h"

#include "Core/System/Application.h"

/* #########################
    LOCAL TESTING FIELDS
######################### */

static Mesh      _TEST_mesh{};
static IVertexShader*     _TEST_pVertexShader{};
static IPixelShader*      _TEST_pPixelShader{};
static ITextureResource*  _TEST_pTextureResource{};
static ISampler*          _TEST_pSampler{};

static VertexBufferHandle   _vertexBufferHandle{};
static IndexBufferHandle    _indexBufferHandle{};

static std::vector<UniquePtr<Mesh>> _list_meshes{};

/* #########################
    LOCAL TESTING FIELDS
######################### */

#pragma warning(disable: 4477)
#pragma warning(disable: 4313)

extern "C" CORE_API Application* CreateApplicationProc() {
  return new Application;
}

float Application::m_runtime{ 0.0f };
float Application::m_deltaTime{ 1.0f };

Application::Application() :
  m_shouldRun     { false },
  m_pRenderModule { nullptr },
  m_pInput        { nullptr },
  m_pSysWindow    { nullptr }
{ }

Application::~Application() {}

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
    // Start by assuming failure
    m_shouldRun = false;
    m_pSysWindow->initialize("Optim Engine - <DX11>");

    SDL_SharedObject* handle = SDL_LoadObject("bin/directx11_ri.dll");

    if (handle == nullptr) {
      printf("Could not load library: %s\n",SDL_GetError());
      Quit();
      return;
    }

    IGraphicsRHI* (*pFactoryGraphicsModule)() = (IGraphicsRHI* (*)())SDL_LoadFunction(handle, "CreateDirect3D11Module");
    /*
    */
    void (*p_testFunction)() = (void (*)())SDL_LoadFunction(handle, "testFunction");

    if (p_testFunction) {
      p_testFunction();
      printf("...TEST FUNCTION NOT FOUND...\n");
    }

    if (pFactoryGraphicsModule == nullptr) {
      printf("Could not initalize factory function for DirectX11 module.");
      SDL_UnloadObject(handle);
      Quit();
      return;
    }
    else {
      m_pRenderModule = pFactoryGraphicsModule();
      m_pRenderModule->Initialize(m_pSysWindow->getSystemPointer());
    }

    //Mesh l_fbxMesh;
    //OptimEditor::loadFbxModel(l_fbxMesh, "Assets/cube.fbx");
    OptimEditor::loadFbxModel(_TEST_mesh, "Assets/monkey.fbx");

    printf("---------------------- APPLICATION.CPP ----------------------\n");
    std::cout << "Vertex Count: " << _TEST_mesh.vertexCount << '\n';
    printf("Index Count: %d\n", _TEST_mesh.indexCount);

    _vertexBufferHandle = m_pRenderModule->createResourceVertexBuffer(_TEST_mesh.vertices, _TEST_mesh.vertexCount);
    _indexBufferHandle = m_pRenderModule->createResourceIndexBuffer(_TEST_mesh.indices, _TEST_mesh.indexCount);

    /*
     * OLD POINTER SYSTEM FOR RESOURCE CREATION
    */

    _TEST_pVertexShader = m_pRenderModule->createVertexShader(TEXT("bin/PhongVertexShader.cso"));
    _TEST_pPixelShader  = m_pRenderModule->createPixelShader(TEXT("bin/PhongPixelShader.cso"));

    // Load image for texture
    Image srcImage;
    FileStream::readPngImage("images/jeff2.png", srcImage);
    _TEST_pTextureResource  = m_pRenderModule->createTextureResource(&srcImage);

    // Create sampler resource
    _TEST_pSampler = m_pRenderModule->createSamplerResource();
    //_hPixelShader = m_pRenderModule->getPixelShader(TEXT("bin/PixelShader.cso"));

    m_shouldRun = true;

    m_pRenderModule->cmdDrawIndexed(_TEST_mesh.indexCount);
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

    if (!m_pSysWindow->loop()) {
      Quit();
      return;
    }

    //m_pRenderModule->bindVertexBuffer(_TEST_mesh.pVertexBuffer);
    //m_pRenderModule->bindIndexBuffer(_TEST_mesh.pIndexBuffer);
    m_pRenderModule->bindVertexShader(_TEST_pVertexShader);
    m_pRenderModule->bindPixelShader(_TEST_pPixelShader);
    m_pRenderModule->bindTexture(_TEST_pTextureResource);
    m_pRenderModule->bindSampler(_TEST_pSampler);

    for (int i = 0; i < _list_meshes.size(); i++) {

    }

    m_pRenderModule->cmdBindVertexBuffer(&_vertexBufferHandle);
    m_pRenderModule->cmdBindIndexBuffer(&_indexBufferHandle);
    m_pRenderModule->cmdDrawIndexed(_TEST_mesh.indexCount);
    m_pRenderModule->draw();

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
  delete(m_pRenderModule);
  delete(m_pSysWindow);
}

CORE_API void OptimEditor::processFile(const char* param_cstrFilePath) 
{
  printf("Application will process droped file from: %s\n", param_cstrFilePath);

  Mesh* l_meshInstance = new Mesh;
  UniquePtr<Mesh> l_uptrMesh(&l_meshInstance);

  OptimEditor::loadFbxModel(*l_uptrMesh, param_cstrFilePath);

  printf("Loaded mesh model vertex count: %d\n", (*l_uptrMesh).vertexCount);
  printf("Loaded mesh model index count: %d\n", (*l_uptrMesh).indexCount);

  _list_meshes.push_back(l_uptrMesh.move());
}
