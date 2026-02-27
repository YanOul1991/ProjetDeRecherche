/*
  Application.cpp
  
  Yanis Oulmane
 */

#pragma once

#include "Core/System/Application.h"

#include "../FileStream.h"
#include "../ModelLoader.h"
#include "../System.h"
#include "../SystemWindow.h"
// #include "Core/System/FileStream.h"
// #include "Core/System/ModelLoader.h"
// #include "Core/System/SystemWindow.h"

#include "Core/Exception/exception.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Object/Camera/Camera.h"
#include "Core/Object/Image/Image.h"
#include "Core/Object/Object.h"
#include "Core/Physics/Physics.h"
#include "Core/Serialization/Parser.h"
#include "Core/Serialization/Serializer.h"
#include "Core/Serialization/Tokenizer.h"
#include "Core/Time/Time.h"
#include "Core/Types/string.h"
#include "Core/Utilities/Pointer/UniquePtr.h"
#include "Core/Utilities/Random/Random.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

// A UniquePtr to the default SystemWindow class object
static UniquePtr<SystemWindow> g_uptrSystemWindow{};

// List of all active Mesh objects in the scene
//
// TODO: Make a new scene module to manage acrive objects
static std::vector<UniquePtr<Mesh>> _list_meshes{};

// A Pointer to a UniquePtr<Mesh> for a selected object in the scene.
static UniquePtr<Mesh>* g_ppSelectedMesh{ nullptr };

// The pointing direction of the translation Gizmo
// when being manipulated.
static float3 controlGizmoDirection{};

// List of Translation Gizmo Mesh objects.
static std::vector<UniquePtr<Mesh>> arrayGizmoSelection;

static PipelineHandle pipelineHandleDefault;    // Default Shaders pipeline
static PipelineHandle pipelineHandleWirframe{}; // Wirframe Shaders pipeline
static PipelineHandle pipelineHandleOutline{};  // Outline Shaders pipeline
static PipelineHandle pipelineHandleGizmo{};    // Gizmo Shaders pipeline
static DepthRTHandle  _handle_depthRT{};

static bool _bool_drawWireframe{ false };
static bool _bool_drawOutline{ false };
static bool _bool_manipulate_selected{ false };

static std::string GetFileExtension(std::string strPath) {
  std::filesystem::path filePath = strPath;
  return filePath.extension().string();
}

static std::string GetRelativePath(std::string strAbsolutePath) {
  std::string droppedFilePath = strAbsolutePath;
  std::string workingDir      = System::GetWorkingDirectory();

  uint64 pos = droppedFilePath.find(workingDir);

  if (pos != std::string::npos) {
    return droppedFilePath.erase(pos, workingDir.length());
  }
  else {
    std::cout << "Could not get relative path of file.\n";
    return std::string();
  }
}

extern "C" CORE_API Application* CreateApplicationProc() {
  return new Application;
}

Application::Application() {
}

Application::~Application() {
}

bool Application::ShouldRun() const {
  return m_shouldRun;
}

void Application::getMainWindowSize(int32* pWidth, int32* pHeight) {
  g_uptrSystemWindow->getWindowSize(pWidth, pHeight);
}

static void getClickSelection(Raycast raycast) {
  // If an object is selected, then prioritize collision detection with gizmo
  // If transform gizmo has indeed touched, then manupulate selected object's
  // transformation and no need to check for other collisions.
  if (g_ppSelectedMesh != nullptr) {
    UniquePtr<Mesh>* targetMesh = Optim::Physics::GetCollision(raycast, arrayGizmoSelection);
    _bool_manipulate_selected   = targetMesh;

    if (targetMesh) {
      controlGizmoDirection = (*targetMesh)->rotation.rotate({ 0, 0, 1 });
      return;
    }
  }

  g_ppSelectedMesh  = Optim::Physics::GetCollision(raycast, _list_meshes);
  _bool_drawOutline = g_ppSelectedMesh;
}

void Application::manageKeyDownEvent(uint32 keycode) {
  // If key is backaspace.
  // Check if a mesh object is selected in the scene.
  // If so find it in the mesh list and if found delete it.
  if (keycode == 8) {
    if (g_ppSelectedMesh != nullptr) {
      std::erase_if(_list_meshes, [](const UniquePtr<Mesh>& element) {
        if (element.address() != nullptr) {
          if (element.address() == g_ppSelectedMesh->address()) {
            g_ppSelectedMesh = nullptr;
            return true;
          }
        }
        return false;
      });
    }
  }
}

void Application::mangeWindowClickEvent(float posX, float posY, int32 buttonID) {
  if (buttonID == 1) {
    int32 width, height;
    getMainWindowSize(&width, &height);

    Raycast raycast = Optim::Physics::ScreenToRaycast(posX, posY, (float)width, (float)height);

    getClickSelection(raycast);
  }

  if (buttonID == 3) {
    _bool_drawWireframe = !_bool_drawWireframe;
  }
}

void Application::manageSysWinMouseUp(float posX, float posY, int32 buttonID) {
  _bool_manipulate_selected = false;
}

void Application::manageWindowResizeEvent(uint32 width, uint32 height) {
  Graphics::RHI()->updateSystemWindowSize(width, height);
}

void Application::manageOnSaveEvent() {
  std::vector<Object*> objectList;
  for (auto& refMesh : _list_meshes) {
    objectList.push_back(refMesh.address());
  }
  Serializer::SaveScene(objectList, "myScene");
}

void Application::manageOnFileDropped(const char* path, float posX, float posY) {
  const std::string fileExtension    = GetFileExtension(path);
  const std::string fileRelativePath = GetRelativePath(path);

  if (fileRelativePath.empty()) {
    String::printf("[Application] WARNING - The dropped file's location must be inside the project folder:\n %s\n", System::GetWorkingDirectory());
    return;
  }

  if (fileExtension == ".png") {
    int32 width;
    int32 height;
    g_uptrSystemWindow->getWindowSize(&width, &height);

    Raycast raycast = Optim::Physics::ScreenToRaycast(posX, posY, (float)width, (float)height);

    UniquePtr<Mesh>* target = Optim::Physics::GetCollision(raycast, _list_meshes);

    if (target) {
      Image pngData;
      FileStream::readPngImage(fileRelativePath.c_str(), pngData);
      (*target)->texturePath   = fileRelativePath;
      (*target)->textureHandle = Graphics::RHI()->createTextureResource(&pngData);
    }
  }
  else if (fileExtension == ".fbx") {
    UniquePtr<Mesh> l_uptrMesh;
    l_uptrMesh.init();

    OptimEditor::loadFbxModel(*l_uptrMesh, fileRelativePath.c_str());

    l_uptrMesh->rotation = { 1.0f, 0.0, 0.0, 0.0f };
    l_uptrMesh->position = { 0, 0, 0 };

    g_ppSelectedMesh = nullptr;

    (*l_uptrMesh).vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(l_uptrMesh->vertices, l_uptrMesh->vertexCount);
    (*l_uptrMesh).indexBufferHandle  = Graphics::RHI()->createResourceIndexBuffer(l_uptrMesh->indices, l_uptrMesh->indexCount);
    (*l_uptrMesh).sourcePath         = fileRelativePath;

    if (!(*l_uptrMesh).texturePath.empty()) {
      Image imgData;
      FileStream::readPngImage((*l_uptrMesh).texturePath.c_str(), imgData);
      (*l_uptrMesh).textureHandle = Graphics::RHI()->createTextureResource(&imgData);
    }
    else {
      (*l_uptrMesh).textureHandle = Graphics::GetDefaultTexture();
    }

    _list_meshes.push_back(l_uptrMesh.move());
    g_ppSelectedMesh = &_list_meshes.back();
  }
  else if (fileExtension == ".oescene") {
    String::printf("[Application] Importing scene object.\n");

    std::vector<Object*> l_registeredObjects;

    Parser meshParser(Token::Tokenize(fileRelativePath));

    while (!meshParser.isEnd()) {
      l_registeredObjects.push_back(reinterpret_cast<Object*>(Parser::CreateObject(meshParser)));
    }

    for (auto& i : l_registeredObjects) {
      if (i->isChildOf(Mesh::StaticTypeInfo())) {

        Mesh* objMesh = reinterpret_cast<Mesh*>(i);

        OptimEditor::loadFbxModel(*objMesh, objMesh->sourcePath.c_str());

        objMesh->vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(objMesh->vertices, objMesh->vertexCount);
        objMesh->indexBufferHandle  = Graphics::RHI()->createResourceIndexBuffer(objMesh->indices, objMesh->indexCount);

        if (objMesh->texturePath.empty()) {
          objMesh->textureHandle = Graphics::GetDefaultTexture();
          printf("The mesh has not texture assigned to it.\n");
        }
        else {
          Image dataImage;
          FileStream::readPngImage(objMesh->texturePath.c_str(), dataImage);
          objMesh->textureHandle = Graphics::RHI()->createTextureResource(&dataImage);
        }

        UniquePtr<Mesh> _meshRef(objMesh);

        _list_meshes.push_back(_meshRef.move());
      }
    }
  }
  else {
    String::printf("Unsupported file extension.\n");
  }
}

void Application::Quit() {
  m_shouldRun = false;
}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart() {
  try {
    Time::onNewFrame();

    System::Initalize();

    printf("Current Working dir: %s\n", System::GetWorkingDirectory());

    // Load system window.
    // Load graphics then display the window.
    g_uptrSystemWindow.init();
    g_uptrSystemWindow->initialize("Optim Engine");
    Graphics::initalize();

    // Subscribe to window delegates
    g_uptrSystemWindow->onSystemWindowClick.subscribe<Application, &Application::mangeWindowClickEvent>(this);
    g_uptrSystemWindow->onWindowResize.subscribe<Application, &Application::manageWindowResizeEvent>(this);
    g_uptrSystemWindow->onSystemWindowMouseUp.subscribe<Application, &Application::manageSysWinMouseUp>(this);
    g_uptrSystemWindow->onSaveEvent.subscribe<Application, &Application::manageOnSaveEvent>(this);
    g_uptrSystemWindow->onKeyDown.subscribe<Application, &Application::manageKeyDownEvent>(this);
    g_uptrSystemWindow->onFileDroppedDelegate.subscribe<Application, &Application::manageOnFileDropped>(this);

    // Display the window
    g_uptrSystemWindow->showWindow();

    ////////////////////////////////////// GIZMO initalization

    arrayGizmoSelection.push_back(UniquePtr<Mesh>());
    arrayGizmoSelection.push_back(UniquePtr<Mesh>());
    arrayGizmoSelection.push_back(UniquePtr<Mesh>());

    for (auto& gizmo : arrayGizmoSelection) {
      gizmo.init();
      OptimEditor::loadFbxModel((*gizmo), "Assets/gizmoSelectionArrow.fbx");
    }

    // Set the vertices color of the X axis facing gizmo to red
    // and forward facing gizmo to blue.
    // No need for Y since by defualt vertice's colors are initalized
    // as green.

    for (size_t i = 0; i < arrayGizmoSelection[0]->vertexCount; i++) {
      arrayGizmoSelection[0]->vertices[i].color = float4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    for (size_t i = 0; i < arrayGizmoSelection[2]->vertexCount; i++) {
      arrayGizmoSelection[2]->vertices[i].color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    for (auto& gizmo : arrayGizmoSelection) {
      gizmo->vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(gizmo->vertices, gizmo->vertexCount);
      gizmo->indexBufferHandle  = Graphics::RHI()->createResourceIndexBuffer(gizmo->indices, gizmo->indexCount);
    }

    arrayGizmoSelection[1]->rotation = Quaternion::fromAxisAngle({ 1, 0, 0 }, -Optim::Constants::pi / 2.0f);
    arrayGizmoSelection[2]->rotation = Quaternion::fromAxisAngle({ 0, 1, 0 }, Optim::Constants::pi / 2.0f);

    // ---------------------------------------------------------------------
    // PIPELINE - PHONG / DEFAULT

    SPipelineDesc pipelineDefaultDesc{};

    pipelineDefaultDesc.vertexShader   = "bin/PhongVertexShader.cso";
    pipelineDefaultDesc.fragmentShader = "bin/PhongPixelShader.cso";

    pipelineDefaultDesc.rasterizerDescription = {
      .fillMode             = ERasterizerFillMode::Solid,
      .cullMode             = ERasterizerCullMode::Back,
      .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
      .depthBias            = 0,
      .slopeScaledDepthBias = 0
    };

    pipelineDefaultDesc.depthStencilDescription = {
      .depthTestEnabled        = true,
      .depthComparisonFunction = EDepthStencilComparisonFunction::Less,
      .depthWriteMask          = EDepthStencilDepthWriteMask::WriteAll
    };

    pipelineDefaultDesc.primitiveTopology = EPipelinePrimitiveTopology::TriangleList;

    pipelineHandleDefault = Graphics::RHI()->createPipeline(&pipelineDefaultDesc);

    // ---------------------------------------------------------------------
    // PIPELINE - WIRFRAME

    SPipelineDesc pipelineWirframeDesc = {
      .vertexShader   = "bin/WireframeVS.cso",
      .fragmentShader = "bin/WireframePS.cso",

      .rasterizerDescription = {
                                .fillMode             = ERasterizerFillMode::Wireframe,
                                .cullMode             = ERasterizerCullMode::None,
                                .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
                                .depthBias            = -1,
                                .slopeScaledDepthBias = -1.0f },

      .depthStencilDescription = {
                                .depthTestEnabled        = true,
                                .depthComparisonFunction = EDepthStencilComparisonFunction::Less,
                                .depthWriteMask          = EDepthStencilDepthWriteMask::WriteAll,
                                },

      .primitiveTopology = EPipelinePrimitiveTopology::TriangleList,
    };

    pipelineHandleWirframe = Graphics::RHI()->createPipeline(&pipelineWirframeDesc);

    // ---------------------------------------------------------------------
    // PIPELINE - OUTLINE

    SPipelineDesc pipelineOutlineDesc = {
      .vertexShader   = "bin/OutlineVS.cso",
      .fragmentShader = "bin/OutlinePS.cso",

      .rasterizerDescription = {
                                .fillMode             = ERasterizerFillMode::Solid,
                                .cullMode             = ERasterizerCullMode::Back,
                                .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
                                .depthBias            = 0,
                                .slopeScaledDepthBias = 0.0f },

      .depthStencilDescription = {
                                .depthTestEnabled        = true,
                                .depthComparisonFunction = EDepthStencilComparisonFunction::Greater,
                                .depthWriteMask          = EDepthStencilDepthWriteMask::WriteNone,
                                },

      .primitiveTopology = EPipelinePrimitiveTopology::TriangleList,
    };
    pipelineHandleOutline = Graphics::RHI()->createPipeline(&pipelineOutlineDesc);

    // ---------------------------------------------------------------------
    // PIPELINE

    SPipelineDesc pipelineGizmoDesc = {
      .vertexShader   = "bin/TransformGizmoVS.cso",
      .fragmentShader = "bin/TransformGizmoPS.cso",

      .rasterizerDescription = {
                                .fillMode             = ERasterizerFillMode::Solid,
                                .cullMode             = ERasterizerCullMode::Back,
                                .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
                                .depthBias            = 0,
                                .slopeScaledDepthBias = 0 },

      .depthStencilDescription = {
                                .depthTestEnabled        = false,
                                .depthComparisonFunction = EDepthStencilComparisonFunction::Less,
                                .depthWriteMask          = EDepthStencilDepthWriteMask::WriteAll,
                                },

      .primitiveTopology = EPipelinePrimitiveTopology::TriangleList,
    };

    pipelineHandleGizmo = Graphics::RHI()->createPipeline(&pipelineGizmoDesc);

    // Create DepthStencil state
    _handle_depthRT = Graphics::RHI()->createDepthRT();

    m_shouldRun = true;

    Time::onFrameEnd();
    printf("--- APPLICATION LOOP BEGIN ---\n");
  }
  catch (const Exception& e) {
    String msg = String::sprintf("[Exception]\n%s\n[Exception Description]\n%s\n[Exception File]\n%s\n", e.type(), e.what(), e.getFile());
    SystemWindow::ShowMessageBox(e.what(), msg.value());
    Quit();
  }
  catch (const std::exception& e) {
    SystemWindow::ShowMessageBox(e.what(), "[Error]");
    Quit();
  }
}

void Application::ApplicationLoop() {
  try {
    Time::onNewFrame();

    if (!g_uptrSystemWindow->loop()) {
      Quit();
      return;
    }

    // Set default render targets and pipeline
    // and textures.

    Graphics::RHI()->cmdSetRenderTargets(&_handle_depthRT);
    Graphics::RHI()->cmdBindPipeline(&pipelineHandleDefault);

    if (g_ppSelectedMesh != nullptr && _bool_manipulate_selected) {
      float mouseDx;
      float mouseDy;

      g_uptrSystemWindow->getMouseDelta(&mouseDx, &mouseDy);

      float mouseMag = magnitude(float2{ mouseDx, mouseDy });

      if (mouseMag != 0) {
        float3 cameraVectorDx = mouseDx * Camera::right;
        float3 cameraVectorDy = -mouseDy * Camera::up;

        float3 worldVector = (mouseDx * Camera::right) + (-mouseDy * Camera::up);

        float dot = dotProduct(normalize(worldVector), controlGizmoDirection);

        float3 vectorToCam = Camera::position - (*g_ppSelectedMesh)->position;

        float distanceToCam = magnitude(vectorToCam);

        (*g_ppSelectedMesh)->position = (*g_ppSelectedMesh)->position + (((distanceToCam / 5.0f) * mouseMag) * 0.01f * (dot * controlGizmoDirection));
      }
    }

    // Iterates through all instanciated mesh objects
    // and render them in the scene
    // If wireframe view is activated also draw their wireframes.

    for (auto& pMesh : _list_meshes) {
      float4x4 worldTransform = pMesh->getWorldMatrix();

      Graphics::RHI()->cmdSetNextMeshTransform(&worldTransform);
      Graphics::RHI()->cmdBindVertexBuffer(&pMesh->vertexBufferHandle);
      Graphics::RHI()->cmdBindIndexBuffer(&pMesh->indexBufferHandle);
      Graphics::RHI()->cmdBindTexture(&pMesh->textureHandle);
      Graphics::RHI()->cmdDrawIndexed(pMesh->indexCount);
    }

    if (_bool_drawWireframe) {
      Graphics::RHI()->cmdBindPipeline(&pipelineHandleWirframe);

      for (UniquePtr<Mesh>& pMesh : _list_meshes) {
        float4x4 worldTransform = pMesh->getWorldMatrix();

        Graphics::RHI()->cmdSetNextMeshTransform(&worldTransform);
        Graphics::RHI()->cmdBindVertexBuffer(&pMesh->vertexBufferHandle);
        Graphics::RHI()->cmdBindIndexBuffer(&pMesh->indexBufferHandle);
        Graphics::RHI()->cmdDrawIndexed(pMesh->indexCount);
      }
    }

    if (g_ppSelectedMesh != nullptr) {
      Graphics::RHI()->cmdBindPipeline(&pipelineHandleOutline);

      const UniquePtr<Mesh>& selectedMesh = (*g_ppSelectedMesh);

      float4x4 worldTransform = selectedMesh->getWorldMatrix();

      Graphics::RHI()->cmdSetNextMeshTransform(&worldTransform);
      Graphics::RHI()->cmdBindVertexBuffer(&selectedMesh->vertexBufferHandle);
      Graphics::RHI()->cmdBindIndexBuffer(&selectedMesh->indexBufferHandle);
      Graphics::RHI()->cmdDrawIndexed(selectedMesh->indexCount);

      Graphics::RHI()->cmdBindPipeline(&pipelineHandleGizmo);
      for (UniquePtr<Mesh>& pGizmo : arrayGizmoSelection) {
        float3 dir = normalize(selectedMesh->position - Camera::position);

        dir = 17 * dir;

        pGizmo->position = Camera::position + dir;

        float4x4 worldTransform = pGizmo->getWorldMatrix();

        Graphics::RHI()->cmdSetNextMeshTransform(&worldTransform);
        Graphics::RHI()->cmdBindVertexBuffer(&pGizmo->vertexBufferHandle);
        Graphics::RHI()->cmdBindIndexBuffer(&pGizmo->indexBufferHandle);
        Graphics::RHI()->cmdDrawIndexed(pGizmo->indexCount);
      }
    }

    // Execute the commands
    Graphics::RHI()->draw();
    Time::onFrameEnd();
  }
  catch (const Exception& e) {
    String msg = String::sprintf("[Exception]\n%s\n[Exception Description]\n%s\n[Exception File]\n%s\n", e.type(), e.what(), e.getFile());
    SystemWindow::ShowMessageBox(e.what(), msg.value());
    Quit();
  }
  catch (const std::exception& e) {
    SystemWindow::ShowMessageBox(e.what(), "[Error]");
    Quit();
  }
}

void Application::ApplicationQuit() {
  printf("Application quitting...\n");
}