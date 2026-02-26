/**
 * Application.cpp
 *
 * Yanis Oulmane
 */

#pragma once

#include "Core/System/Application.h"

#include "Core/Exception/exception.h"
#include "Core/Graphics/Graphics.h"
#include "Core/Graphics/IGraphicsRHI.h"
#include "Core/Graphics/Mesh.h"
#include "Core/Object/Camera/Camera.h"
#include "Core/Object/Image/Image.h"
#include "Core/Object/Object.h"
#include "Core/Serialization/Parser.h"
#include "Core/Serialization/Serializer.h"
#include "Core/Serialization/Tokenizer.h"
#include "Core/System/FileStream.h"
#include "Core/System/ModelLoader.h"
#include "Core/System/SystemWindow.h"
#include "Core/Time/Time.h"
#include "Core/Types/Color.h"
#include "Core/Types/string.h"
#include "Core/Utilities/Pointer/UniquePtr.h"
#include "Core/Utilities/Random/Random.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#if defined(_WIN32) | defined(_WIN64)
  #include "Core/Defines/Windows/windowsAPI.h"
#endif // _WIN32 || _WIN64

// WINDOW API

static const std::string staticWorkingDirectory = std::filesystem::current_path().string().append("\\");

static UniquePtr<SystemWindow> g_uptrSystemWindow{};

static std::vector<UniquePtr<Mesh>> _list_meshes{};
static std::vector<UniquePtr<Mesh>> _list_Rays{};

static UniquePtr<Mesh>* g_ppSelectedMesh{ nullptr };

static float3 controlGizmoDirection{};

static std::vector<UniquePtr<Mesh>> arrayGizmoSelection;

static std::vector<Mesh> g_objectMeshes;

static PipelineHandle _newPipelineHandleTest;

// static PipelineHandle _handlePipeline{};
static PipelineHandle _handlePipelineWirframeView{};
static PipelineHandle _handlePipelineOutline{};
static PipelineHandle _handlePipelineLineRendering{};

static TextureResourceHandle _handleTextureResource{};

static DepthRTHandle _handle_depthRT{};

static bool _bool_drawWireframe{ false };
static bool _bool_drawOutline{ false };
static bool _bool_manipulate_selected{ false };

extern "C" CORE_API Application* CreateApplicationProc() {
  return new Application;
}

float Application::m_runtime{ 0.0f };
float Application::m_deltaTime{ 1.0f };

Application::Application() {
}

Application::~Application() {
}

bool Application::ShouldRun() const {
  return m_shouldRun;
}

float Application::getRuntime() {
  return m_runtime;
}

float Application::getDeltaTime() {
  return m_deltaTime;
}

void Application::getMainWindowSize(int32* pWidth, int32* pHeight) {
  g_uptrSystemWindow->getWindowSize(pWidth, pHeight);
}

static void getClickSelection(float3 rayOrigin, float3 rayFarPosition) {
  float3 rayDirection = normalize(rayFarPosition - rayOrigin);

  // If an object is selected, then prioritize the selected
  if (g_ppSelectedMesh != nullptr) {

    for (auto& mesh : arrayGizmoSelection) {
      float4x4 worldTransform = mesh->getWorldMatrix().transpose();

      for (int i = 0; i < (int)((float)mesh->indexCount / 3); i++) {
        float tHit = 0;

        float3 O = float3{ 0, 0, 0 };
        float3 D = rayDirection;

        float4 v0Local = Optim::Mathematics::getFloat4FromFloat3(mesh->vertices[mesh->indices[3 * i]].position);
        float4 v1Local = Optim::Mathematics::getFloat4FromFloat3(mesh->vertices[mesh->indices[3 * i + 1]].position);
        float4 v2Local = Optim::Mathematics::getFloat4FromFloat3(mesh->vertices[mesh->indices[3 * i + 2]].position);

        float3 v0 = Optim::Mathematics::getFloat3Part(worldTransform * v0Local);
        float3 v1 = Optim::Mathematics::getFloat3Part(worldTransform * v1Local);
        float3 v2 = Optim::Mathematics::getFloat3Part(worldTransform * v2Local);

        // Compute triangle edges
        float3 e1 = v1 - v0;
        float3 e2 = v2 - v0;

        constexpr float EPS = 1E-8F;

        float3 p   = cross(rayDirection, e2); // Get Vector perpendicular to ray direction and second triangle edge
        float  det = dotProduct(e1, p);       // Get determinant to check if ray is parallel to triangle

        if (fabsf(det) < EPS) {
          // Ray is parralel to the triangle
          _bool_drawOutline = false;
          continue;
        }

        float invDet = 1.0f / det;

        float3 t = rayOrigin - v0; // Vector from triangle first vertex to ray origin

        // Compute barycentric coordinate u
        float u = dotProduct(t, p) * invDet;

        if (u < 0.0f || u > 1.0f) {
          _bool_drawOutline = false;
          continue;
        }

        float3 q = cross(t, e1);

        float v = dotProduct(rayDirection, q) * invDet;
        if (v < 0.0f || u + v > 1.0f) {
          _bool_drawOutline = false;
          continue;
        }

        tHit = dotProduct(e2, q) * invDet;

        if (tHit > EPS) {
          _bool_drawOutline     = true;
          controlGizmoDirection = mesh->rotation.rotate({ 0, 0, 1 });
          // printf("GizmoTouched!\n");
          _bool_manipulate_selected = true;
          // controlGizmoDirection.print();
          //(*g_ppSelectedMesh)->position = (*g_ppSelectedMesh)->position + (0.25f * controlGizmoDirection);
          return;
        }
      } // for loop end - single mesh indices loop
    } // For loop end - gizmos list iteration
  }

  for (auto& mesh : _list_meshes) {
    float4x4 worldTransform = mesh->getWorldMatrix().transpose();

    for (int i = 0; i < (int)((float)mesh->indexCount / 3); i++) {
      float tHit = 0;

      float3 O = float3{ 0, 0, 0 };
      float3 D = rayDirection;

      float4 v0Local = Optim::Mathematics::getFloat4FromFloat3(mesh->vertices[mesh->indices[3 * i]].position);
      float4 v1Local = Optim::Mathematics::getFloat4FromFloat3(mesh->vertices[mesh->indices[3 * i + 1]].position);
      float4 v2Local = Optim::Mathematics::getFloat4FromFloat3(mesh->vertices[mesh->indices[3 * i + 2]].position);

      float3 v0 = Optim::Mathematics::getFloat3Part(worldTransform * v0Local);
      float3 v1 = Optim::Mathematics::getFloat3Part(worldTransform * v1Local);
      float3 v2 = Optim::Mathematics::getFloat3Part(worldTransform * v2Local);

      // printf("Triangle: V1(%f, %f, %f) | V2(%f, %f, %f) | V3(%f, %f, %f)\n", v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);

      // Compute triangle edges
      float3 e1 = v1 - v0;
      float3 e2 = v2 - v0;

      constexpr float EPS = 1E-8F;

      float3 p   = cross(rayDirection, e2); // Get Vector perpendicular to ray direction and second triangle edge
      float  det = dotProduct(e1, p);       // Get determinant to check if ray is parallel to triangle

      if (fabsf(det) < EPS) {
        // Ray is parralel to the triangle
        _bool_drawOutline = false;
        continue;
      }

      float invDet = 1.0f / det;

      float3 t = rayOrigin - v0; // Vector from triangle first vertex to ray origin

      // Compute barycentric coordinate u
      float u = dotProduct(t, p) * invDet;

      if (u < 0.0f || u > 1.0f) {
        _bool_drawOutline = false;
        continue;
      }

      float3 q = cross(t, e1);

      float v = dotProduct(rayDirection, q) * invDet;
      if (v < 0.0f || u + v > 1.0f) {
        _bool_drawOutline = false;
        continue;
      }

      tHit = dotProduct(e2, q) * invDet;

      if (tHit > EPS) {
        _bool_drawOutline = true;
        // printf("Collision with mesh detected distance: %f\n", tHit);
        g_ppSelectedMesh = &mesh;

        // printf("Selected Mesh info:\n");
        // printFields((uint8*)(*g_ppSelectedMesh).address(), (*g_ppSelectedMesh)->GetTypeInfo(), 2);
        return;
      }
    } // for loop end - single mesh indices loop
  } // For loop end - mesh list iteration

  // printf("No collision detected with any mesh.\n");
  g_ppSelectedMesh = nullptr;
}

void Application::manageKeyDownEvent(uint32 keycode) {
  // std::cout << "[Application] Keydown event : " << keycode << "\n";

  if (keycode == 8) {
    // std::cout << "[Application] Backspace clicked.\n";

    if (g_ppSelectedMesh != nullptr) {
      std::erase_if(_list_meshes, [](const UniquePtr<Mesh>& element) {
        // printf("Comparaison: %p : %p\n", g_ppSelectedMesh->address(), element.address());
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
  if (buttonID == 3) {
    _bool_drawWireframe = !_bool_drawWireframe;
  }

  if (buttonID != 1) {
    return;
  }

  int32 width{};
  int32 height{};

  getMainWindowSize(&width, &height);

  float ndcX = (2 * (posX) / static_cast<float>(width)) - 1.0f;
  float ndcY = 1.0f - (2 * (posY) / static_cast<float>(height));

  float4 nearPoint = {
    ndcX,
    ndcY,
    0.0f,
    1.0f
  };

  float4 farPoint = {
    ndcX,
    ndcY,
    1.0f,
    1.0f
  };

  float4x4 viewMatrix = Camera::getViewMatrix();

  viewMatrix = Optim::Mathematics::getMatrixTranspose(viewMatrix);

  float a = (float)width / float(height);

  constexpr float fov    = mathConst::PI / 3.0f;
  constexpr float n      = 0.1f;
  constexpr float f      = 1000.0f;
  float           yScale = 1.0f / (tan(fov / 2.0f));

  float4x4 perspectiveMatrix = float4x4{
    yScale / a, 0, 0, 0, 0, yScale, 0, 0, 0, 0, f / (n - f), -1, 0, 0, (n * f) / (n - f), 0
  };

  perspectiveMatrix = Optim::Mathematics::getMatrixTranspose(perspectiveMatrix);

  float4x4 viewProjInverse = Optim::Mathematics::getMatrixInverse(perspectiveMatrix * viewMatrix);

  float4 posNear = viewProjInverse * nearPoint;
  float4 posFar  = viewProjInverse * farPoint;

  float3 rayOrigin = {
    posNear.x / posNear.w,
    posNear.y / posNear.w,
    posNear.z / posNear.w,
  };

  float3 rayFarPosition = {
    posFar.x / posFar.w,
    posFar.y / posFar.w,
    posFar.z / posFar.w,
  };

  float3 rayDirection = normalize(rayFarPosition - rayOrigin);

  getClickSelection(rayOrigin, rayFarPosition);
}

void Application::manageSysWinMouseUp(float posX, float posY, int32 buttonID) {
  _bool_manipulate_selected = false;
}

void Application::manageWindowResizeEvent(uint32 width, uint32 height) {
  Graphics::RHI()->updateSystemWindowSize(width, height);
}

/**
 * @brief
 * Response to a SystemWindow's onSaveEvent being triggered.
 */
void Application::manageOnSaveEvent() {
  printf("Saving...\n");

  std::vector<Object*> objectList;

  for (auto& refMesh : _list_meshes) {
    objectList.push_back(refMesh.address());
  }

  Serializer::SaveScene(objectList, "myScene");

  printf("Scene saved!\n");
}

void Application::Quit() {
  m_shouldRun = false;
}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart() {
  try {
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

    // //////////////////////////////////// GIZMO initalization - END

    //////////////////////////////// TEST NEW PIPELINE SYSTEM

    
    // IMPORTANT: 
    // With new shader reflection implementation started,
    // this system of manually writing inputs could be
    // removed.

    SPipelineInputDescription inputPosition{
      .name       = "POSITION",
      .format     = EGraphicsFormat::r32g32b32_float,
      .inputSlot  = 0,
      .inputUsage = EInputUsageSlot::position
    };
    SPipelineInputDescription inputUv{
      .name       = "TEXCOORD",
      .format     = EGraphicsFormat::r32g32_float,
      .inputSlot  = 1,
      .inputUsage = EInputUsageSlot::textCoord
    };
    SPipelineInputDescription inputNorm{
      .name       = "NORMAL",
      .format     = EGraphicsFormat::r32g32b32_float,
      .inputSlot  = 2,
      .inputUsage = EInputUsageSlot::normal
    };

    std::vector<SPipelineInputDescription> pipeLineInputs = {
      inputPosition, inputUv, inputNorm
    };

    // ---------------------------------------------------------------------
    // PIPELINE - PHONG / DEFAULT

    SPipelineDesc testBasicPipelineDesc{};

    testBasicPipelineDesc.vertexShader   = "bin/PhongVertexShader.cso";
    testBasicPipelineDesc.fragmentShader = "bin/PhongPixelShader.cso";

    testBasicPipelineDesc.rasterizerDescription = {
      .fillMode             = ERasterizerFillMode::Solid,
      .cullMode             = ERasterizerCullMode::Back,
      .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
      .depthBias            = 0,
      .slopeScaledDepthBias = 0
    };

    testBasicPipelineDesc.depthStencilDescription = {
      .depthTestEnabled        = true,
      .depthComparisonFunction = EDepthStencilComparisonFunction::Less,
      .depthWriteMask          = EDepthStencilDepthWriteMask::WriteAll
    };

    testBasicPipelineDesc.inputs = pipeLineInputs;

    testBasicPipelineDesc.primitiveTopology = EPipelinePrimitiveTopology::TriangleList;

    testBasicPipelineDesc.inputs = pipeLineInputs;

    _newPipelineHandleTest = Graphics::RHI()->createPipeline(&testBasicPipelineDesc);

    // ---------------------------------------------------------------------
    // PIPELINE - WIRFRAME

    SPipelineDesc l_wirframePipelineDesc = {
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

      .inputs = pipeLineInputs
    };

    _handlePipelineWirframeView = Graphics::RHI()->createPipeline(&l_wirframePipelineDesc);

    // ---------------------------------------------------------------------
    // PIPELINE - OUTLINE

    SPipelineDesc l_outlinePipelineDesc = {
      .vertexShader   = "bin/OutlineVS.cso",
      .fragmentShader = "bin/OutlinePS.cso",

      .rasterizerDescription = {
                                .fillMode             = ERasterizerFillMode::Solid,
                                .cullMode             = ERasterizerCullMode::Back,
                                .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
                                .depthBias            = -1,
                                .slopeScaledDepthBias = -10.0f },

      .depthStencilDescription = {
                                .depthTestEnabled        = true,
                                .depthComparisonFunction = EDepthStencilComparisonFunction::Greater,
                                .depthWriteMask          = EDepthStencilDepthWriteMask::WriteNone,
                                },

      .primitiveTopology = EPipelinePrimitiveTopology::TriangleList,

      .inputs = pipeLineInputs
    };
    _handlePipelineOutline = Graphics::RHI()->createPipeline(&l_outlinePipelineDesc);

    // ---------------------------------------------------------------------
    // PIPELINE

    SPipelineInputDescription pipelineGizmoInputPosition{
      .name       = "POSITION",
      .format     = EGraphicsFormat::r32g32b32_float,
      .inputSlot  = 0,
      .inputUsage = EInputUsageSlot::position
    };

    SPipelineInputDescription pipelineGizmoInputColor{
      .name       = "COLOR",
      .format     = EGraphicsFormat::r32g32b32a32_float,
      .inputSlot  = 1,
      .inputUsage = EInputUsageSlot::color
    };

    std::vector<SPipelineInputDescription> pipelineGizmoInput = {
      pipelineGizmoInputPosition,
      pipelineGizmoInputColor
    };

    SPipelineDesc l_pipelineLineDesc = {
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

      .inputs = pipelineGizmoInput
    };
    _handlePipelineLineRendering = Graphics::RHI()->createPipeline(&l_pipelineLineDesc);

    /*
     */

    // Create DepthStencil state
    _handle_depthRT = Graphics::RHI()->createDepthRT();

    // Load image for texture
    Image srcImage;
    FileStream::readPngImage("images/flat.png", srcImage);
    _handleTextureResource = Graphics::RHI()->createTextureResource(&srcImage);

    /// ---------------------------------------------------------------------------
    /// ------------------------------ LOADING SCENE ------------------------------

    std::vector<Object*> l_registeredObjects;
    // printf("Loading scene...\n");
    Parser meshParser(Token::Tokenize("Scenes/myScene.oescene"));
    // printf("Pasing files...\n");
    while (!meshParser.isEnd()) {
      l_registeredObjects.push_back(reinterpret_cast<Object*>(Parser::CreateObject(meshParser)));
    }
    // printf("Creating objects...\n");
    for (auto& i : l_registeredObjects) {
      if (i->isChildOf(Mesh::StaticTypeInfo())) {
        // printf("Creating mesh...\n");
        Mesh* objMesh = reinterpret_cast<Mesh*>(i);
        // printf("Creating loading model...\n");
        OptimEditor::loadFbxModel(*objMesh, objMesh->sourcePath.c_str());
        // printf("Loading creating buffers...\n");
        objMesh->vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(objMesh->vertices, objMesh->vertexCount);
        objMesh->indexBufferHandle  = Graphics::RHI()->createResourceIndexBuffer(objMesh->indices, objMesh->indexCount);
        // printf("Making unique...\n");
        UniquePtr<Mesh> _meshRef(objMesh);
        // printf("Making pushing to list...\n");
        _list_meshes.push_back(_meshRef.move());
        // printf("Mesh added...\n");
      }
    }

    m_shouldRun = true;

    printf("--- APPLICATION LOOP BEGIN ---\n");
  }
  catch (const Exception& e) {
    String fullMessage = String(e.whatDescriptive());
    MessageBoxA(0, fullMessage.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e) {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...) {
    MessageBoxA(0, "Unknown details, for exception thrown", "Exception...", MB_OK + MB_ICONEXCLAMATION);
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
    Graphics::RHI()->cmdBindPipeline(&_newPipelineHandleTest);
    Graphics::RHI()->cmdBindTexture(&_handleTextureResource);

    if (g_ppSelectedMesh != nullptr && _bool_manipulate_selected) {
      float mouseDx{};
      float mouseDy{};

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
    //
    // If wireframe view is activated also draw their conressponding wirferame.

    for (auto& pMesh : _list_meshes) {
      float4x4 worldTransform = pMesh->getWorldMatrix();

      Graphics::RHI()->cmdSetNextMeshTransform(&worldTransform);
      Graphics::RHI()->cmdBindVertexBuffer(&pMesh->vertexBufferHandle);
      Graphics::RHI()->cmdBindIndexBuffer(&pMesh->indexBufferHandle);
      Graphics::RHI()->cmdDrawIndexed(pMesh->indexCount);
    }

    // Draw wireframe for all meshes if required

    if (_bool_drawWireframe) {
      Graphics::RHI()->cmdBindPipeline(&_handlePipelineWirframeView);

      for (UniquePtr<Mesh>& pMesh : _list_meshes) {
        float4x4 worldTransform = pMesh->getWorldMatrix();

        Graphics::RHI()->cmdSetNextMeshTransform(&worldTransform);
        Graphics::RHI()->cmdBindVertexBuffer(&pMesh->vertexBufferHandle);
        Graphics::RHI()->cmdBindIndexBuffer(&pMesh->indexBufferHandle);
        Graphics::RHI()->cmdDrawIndexed(pMesh->indexCount);
      }
    }

    if (g_ppSelectedMesh != nullptr) {

      Graphics::RHI()->cmdBindPipeline(&_handlePipelineOutline);

      const UniquePtr<Mesh>& selectedMesh = (*g_ppSelectedMesh);

      float4x4 worldTransform = (*g_ppSelectedMesh)->getWorldMatrix();

      Graphics::RHI()->cmdSetNextMeshTransform(&worldTransform);
      Graphics::RHI()->cmdBindVertexBuffer(&(*g_ppSelectedMesh)->vertexBufferHandle);
      Graphics::RHI()->cmdBindIndexBuffer(&(*g_ppSelectedMesh)->indexBufferHandle);
      Graphics::RHI()->cmdDrawIndexed((*g_ppSelectedMesh)->indexCount);

      Graphics::RHI()->cmdBindPipeline(&_handlePipelineLineRendering);
      for (UniquePtr<Mesh>& pGizmo : arrayGizmoSelection) {
        float3 dir = normalize((*g_ppSelectedMesh)->position - Camera::position);

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
    m_deltaTime = static_cast<float>(Time::getDeltaTime());
    m_runtime  += m_deltaTime;
  }
  catch (const Exception& e) {
    String msg = String::sprintf("[Exception]\n%s\n[Exception Description]\n%s\n[Exception File]\n%s\n", e.type(), e.what(), e.getFile());

    // String fullMessage = String(e.type());
    // fullMessage       += String("\n\n[Description]\n") + String(e.what()) + String("\n[File]\n") + e.getFile();

    MessageBoxA(0, msg.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e) {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...) {
    MessageBoxA(0, "Unknown details, for exception thrown", "Exception...", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
}

void Application::ApplicationQuit() {
  printf("Application quitting...\n");
}

void OptimEditor::processFile(const char* param_cstrFilePath) {
  std::string droppedFilePath = param_cstrFilePath;

  std::filesystem::path filePath = param_cstrFilePath;

  const std::string fileExtension = filePath.extension().string();

  uint64 pos = droppedFilePath.find(staticWorkingDirectory);

  if (pos != std::string::npos) {
    droppedFilePath.erase(pos, staticWorkingDirectory.length());
    std::cout << "Relative file location: " << droppedFilePath << "\n";
  }
  else {
    std::cout << "Could not get relative path of file.\n";
  }

  if (fileExtension == ".fbx") {
    std::cout << "Dropped fbx file.\n";
    UniquePtr<Mesh> l_uptrMesh;

    l_uptrMesh.init();

    OptimEditor::loadFbxModel(*l_uptrMesh, droppedFilePath.c_str());

    l_uptrMesh->rotation = { 1.0f, 0.0, 0.0, 0.0f };
    l_uptrMesh->position = { 0, 0, 0 };

    g_ppSelectedMesh = nullptr;

    (*l_uptrMesh).vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(l_uptrMesh->vertices, l_uptrMesh->vertexCount);
    (*l_uptrMesh).indexBufferHandle  = Graphics::RHI()->createResourceIndexBuffer(l_uptrMesh->indices, l_uptrMesh->indexCount);
    (*l_uptrMesh).sourcePath         = droppedFilePath;

    _list_meshes.push_back(l_uptrMesh.move());

    g_ppSelectedMesh = &_list_meshes.back();
  }
  else if (fileExtension == ".png") {
    std::cout << "Dropped png file\n";
  }
  else {
    std::cout << "Unkownd or unsupported file type.\n";
  }
}