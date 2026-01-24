/* ======================================================================================
 *  Application.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Graphics/IGraphicsModule.h"
#include "Core/Input/Input.h"
#include "Core/Time/Time.h"
#include "Core/Types/Color.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/System/FileStream.h"
#include "Core/System/SystemWindow.h"
#include "Core/Object/Image/Image.h"
#include "Core/System/Application.h"
#include "Core/Object/Object.h"
#include "Core/Utilities/Random/Random.h"

#include "Core/System/ModelLoader.h"

#include "ThirdParty/SDL3/SDL.h"

#include <iostream>
#include <sstream>
#include <format>
#include <memory>
#include <fstream>
#include <string>

#include "Core/Graphics/Mesh.h"
#include "Core/Graphics/Resource/IVertexShader.h"
#include "Core/Graphics/Resource/IPixelShader.h"
#include "Core/Graphics/Resource/ITextureResource.h"
#include "Core/Graphics/Resource/ISampler.h"

/* #########################
    LOCAL TESTING FIELDS
######################### */

static Mesh      _TEST_mesh{};
static IVertexShader*     _TEST_pVertexShader{};
static IPixelShader*      _TEST_pPixelShader{};
static ITextureResource*  _TEST_pTextureResource{};
static ISampler*          _TEST_pSampler{};

static SGraphicResourceHandle    _hPixelShader;

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
    m_pSysWindow->initialize(TEXT("OptimEngine"));

    SDL_SharedObject* handle = SDL_LoadObject("bin/directx11_ri.dll");

    if (handle == nullptr) {
      printf("Could not load library: %s\n",SDL_GetError());
      Quit();
      return;
    }

    IGraphicsModule* (*pFactoryGraphicsModule)() = (IGraphicsModule* (*)())SDL_LoadFunction(handle, "CreateDirect3D11Module");
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

    Mesh l_fbxMesh;
    OptimEditor::loadFbxModel(l_fbxMesh, "Assets/cube.fbx");

    printf("---------------------- APPLICATION.CPP ----------------------\n");
    std::cout << "Vertex Count: " << l_fbxMesh.vertexCount << '\n';
    //l_fbxMesh.vertices[0].position = {0, 0, 0};
    //l_fbxMesh.vertices[0].uvCoord = {0, 0};
    //l_fbxMesh.vertices[0].normal = {0, 0, 0};

    for (int i = 0; i < l_fbxMesh.vertexCount; i++) {
      printf("Vertex %003d\n", i);
      l_fbxMesh.vertices[i].print();
      printf("\n");
    }
    //l_fbxMesh.vertices[0].print();
    //std::cout << "Index Count: " << _TEST_mesh.indexCount << '\n';

    /////////////////////////////////       TESTING FUNCTIONALITIES

    Mesh::setMeshFromOBJFile(_TEST_mesh, "Assets/jeffSphereSmooth.obj");


    //for (size_t i = 0; i < _TEST_mesh.vertexCount; i += 3) {
    //  printf("Index: (%d, %d, %d)\n", _TEST_mesh.indices[i], _TEST_mesh.indices[i + 1], _TEST_mesh.indices[i + 2]);
    //}
    //for (size_t i = 0; i < _TEST_mesh.vertexCount; i++) {
    //  printf("Vertex position: (%f, %f, %f)\n", _TEST_mesh.vertices[i].position.x, _TEST_mesh.vertices[i].position.y, _TEST_mesh.vertices[i].position.z);
    //}

    _TEST_mesh.pVertexBuffer = m_pRenderModule->createVertexBuffer(_TEST_mesh.vertices, _TEST_mesh.vertexCount);
    _TEST_mesh.pIndexBuffer  = m_pRenderModule->createIndexBuffer(_TEST_mesh.indices, _TEST_mesh.indexCount);

    _TEST_pVertexShader = m_pRenderModule->createVertexShader(TEXT("bin/PhongVertexShader.cso"));
    _TEST_pPixelShader  = m_pRenderModule->createPixelShader(TEXT("bin/PhongPixelShader.cso"));

    // Load image for texture
    Image srcImage;
    FileStream::readPngImage("images/jeff.png", srcImage);
    _TEST_pTextureResource  = m_pRenderModule->createTextureResource(&srcImage);

    // Create sampler resource
    _TEST_pSampler = m_pRenderModule->createSamplerResource();
    _hPixelShader = m_pRenderModule->getPixelShader(TEXT("bin/PixelShader.cso"));

    //printf("Gen of generated pixel resource [%02d]\n", _hPixelShader.generation);

    m_shouldRun = true;

    //IGraphicsResource::printTypes();
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

    if (m_pRenderModule) {
      /*
      DrawCommand testCommand{};
      testCommand.pixelShader = _hPixelShader;
      m_pRenderModule->setDrawCommand(testCommand);
      */

      m_pRenderModule->bindVertexBuffer(_TEST_mesh.pVertexBuffer);
      m_pRenderModule->bindIndexBuffer(_TEST_mesh.pIndexBuffer);
      m_pRenderModule->bindVertexShader(_TEST_pVertexShader);
      m_pRenderModule->bindPixelShader(_TEST_pPixelShader);
      m_pRenderModule->bindTexture(_TEST_pTextureResource);
      m_pRenderModule->bindSampler(_TEST_pSampler);
      m_pRenderModule->draw();
    }

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