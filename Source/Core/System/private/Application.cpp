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

#include "Core/Object/Camera/Camera.h"


/*
* STATIC GLOBAL VARIABLES 
* 
* >>>> THESE ARE FOR TESTING PURPOSES <<<<
* 
*/

static ITextureResource*    _TEST_pTextureResource  {};
static ISampler*            _TEST_pSampler          {};

static UniquePtr<SystemWindow>      g_uptrSystemWindow{};

static std::vector<UniquePtr<Mesh>> _list_meshes{};
static std::vector<UniquePtr<Mesh>> _list_Rays{};

static Mesh _worldGridMesh{};

//static Mesh _meshLineRender{};

static PipelineHandle         _handlePipeline{};
static PipelineHandle         _handlePipelineWirframeView{};
static PipelineHandle         _handlePipelineOutline{};
static PipelineHandle         _handlePipelineLineRendering{};

static DepthRTHandle          _handle_depthRT{};
static VertexShaderHandle     _handle_vertexShader{};
static FragmentShaderHandle   _handle_fragmentShader{};

static bool _bool_drawWireframe {false};
static bool _bool_drawOutline   {false};

// ////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////

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

void Application::getMainWindowSize(int32* pWidth, int32* pHeight)
{
  g_uptrSystemWindow->getWindowSize(pWidth, pHeight);
}

void Application::testFunc_eventSubscribtion(int32 buttonId, int32 windowID)
{
  if (buttonId == 3) {
    _bool_drawWireframe = !_bool_drawWireframe;
  }
}

void Application::mangeWindowClickEvent(float posX, float posY, int32 buttonID)
{
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

  float4x4 viewMatrix = {
    Camera::right.x,  Camera::up.x, -Camera::forward.x, 0,
    Camera::right.y,  Camera::up.y, -Camera::forward.y, 0,
    Camera::right.z,  Camera::up.z, -Camera::forward.z, 0,
    -dotProduct(Camera::right, Camera::position), -dotProduct(Camera::up, Camera::position), dotProduct(Camera::forward, Camera::position), 1,
  };

  viewMatrix = Optim::Mathematics::getMatrixTranspose(viewMatrix);


  float a = (float)width / float(height);

  constexpr float fov = mathConst::PI / 3.0f;
  constexpr float n   = 0.1f;
  constexpr float f   = 1000.0f;
  float yScale = 1.0f / (tan(fov / 2.0f));

  float4x4 perspectiveMatrix = float4x4 {
    yScale / a, 0, 0, 0,
    0, yScale, 0, 0,
    0, 0, f / (n - f), -1,
    0, 0, (n * f) / (n - f), 0
  };

  perspectiveMatrix = Optim::Mathematics::getMatrixTranspose(perspectiveMatrix);

  float4x4 viewProjInverse = Optim::Mathematics::getMatrixInverse(perspectiveMatrix * viewMatrix);

  float4 posNear  = viewProjInverse * nearPoint;
  float4 posFar   = viewProjInverse * farPoint;

	float3 rayOrigin    = { posNear.x / posNear.w, posNear.y / posNear.w,  posNear.z / posNear.w, };
	float3 rayFarPosition = { posFar.x / posFar.w, posFar.y / posFar.w, posFar.z / posFar.w, };

  float3 rayDirection = normalize(rayFarPosition - rayOrigin);

  // ==================================================
  // RAYCAST VISUALISATION
  // ==================================================

  /*
	UniquePtr<Mesh> _meshInstance{};
	_meshInstance.init();

	_meshInstance->vertices = new Vertex[2];

	_meshInstance->vertices[0] = Vertex{
		.position = rayOrigin,
		.uvCoord = {0, 0},
		.normal = {0, 0, 0}
	};
	_meshInstance->vertices[1] = Vertex{
		.position = rayFarPosition,
		.uvCoord = {0, 0},
		.normal = {0, 0, 0}
	};

	_meshInstance->indices = new uint32[2]{
		0, 1
	};

	_meshInstance->vertexCount = 2;
	_meshInstance->indexCount = 2;

	_meshInstance->vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(_meshInstance->vertices, 2);
	_meshInstance->indexBufferHandle = Graphics::RHI()->createResourceIndexBuffer(_meshInstance->indices, 2);

	_list_Rays.push_back(_meshInstance.move());
  */
  
  // ==================================================
  // RAYCAST VISUALISATION - END
  // ==================================================

  /*
  printf("Origin Point = (%f, %f, %f)\n", originPoint.x, originPoint.y, originPoint.z);
  printf("Far Point    = (%f, %f, %f)\n", directionPoint.x, directionPoint.y, directionPoint.z);
  printf("Far Point    = (%f, %f, %f)\n", rayDirection.x, rayDirection.y, rayDirection.z);
  */

  /* -----------------------------------
  * RAY HIT 
  ----------------------------------- */

  for (auto& mesh : _list_meshes) {
    for (int i = 0; i < (int)((float)mesh->indexCount / 3); i++) {
      /* =========================================
      * Visualize triangles test
      ========================================= */

      /*
			UniquePtr<Mesh> _meshInstance{};
			_meshInstance.init();

			_meshInstance->vertices = new Vertex[3];

			_meshInstance->vertices[0] = Vertex{
				.position = mesh->vertices[mesh->indices[3 * i]].position,
				.uvCoord = {0, 0},
				.normal = {0, 0, 0}
			};
			_meshInstance->vertices[1] = Vertex{
				.position = mesh->vertices[mesh->indices[3 * i + 1]].position,
				.uvCoord = {0, 0},
				.normal = {0, 0, 0}
			};
			_meshInstance->vertices[2] = Vertex{
				.position = mesh->vertices[mesh->indices[3 * i + 2]].position,
				.uvCoord = {0, 0},
				.normal = {0, 0, 0}
			};

			_meshInstance->indices = new uint32[4]{
				0, 1, 2, 0
			};

			_meshInstance->vertexCount = 3;
			_meshInstance->indexCount = 4;

			_meshInstance->vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(_meshInstance->vertices, 3);
			_meshInstance->indexBufferHandle = Graphics::RHI()->createResourceIndexBuffer(_meshInstance->indices, 4);

			_list_Rays.push_back(_meshInstance.move());
      */

      /* =========================================
      * END - Visualize triangles test
      ========================================= */


      float tHit = 0;

      float3 O  = float3 {0, 0, 0};
      float3 D  = rayDirection;

      float3 v0 = mesh->vertices[mesh->indices[3 * i]].position;
      float3 v1 = mesh->vertices[mesh->indices[3 * i + 1]].position;
      float3 v2 = mesh->vertices[mesh->indices[3 * i + 2]].position;

      //printf("Triangle: V1(%f, %f, %f) | V2(%f, %f, %f) | V3(%f, %f, %f)\n", v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);

      // Compute triangle edges
      float3 e1 = v1 - v0;
      float3 e2 = v2 - v0;

      constexpr float EPS = 1E-8F;

      float3 p  = cross(rayDirection, e2);  // Get Vector perpendicular to ray direction and second triangle edge
      float det = dotProduct(e1, p);        // Get determinant to check if ray is parallel to triangle 

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

      if (tHit > EPS ) {
        _bool_drawOutline = true;
        printf("Collision with mesh detected.\n");

        // If mesh is indeed clicked no need to continue loop for other triangles
        break;
      }
    } // for loop end - single mesh indices loop
  } // For loop end - mesh list iteration
}

void Application::Quit() 
{
  m_shouldRun = false; 
}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart()
{
  try {
    // Load system window.
    // Load graphics then display the window.
    g_uptrSystemWindow.init();
    g_uptrSystemWindow->initialize("Optim Engine");
    Graphics::initalize();
    g_uptrSystemWindow->showWindow();
    g_uptrSystemWindow->onWindowClick.subscribe<Application, &Application::testFunc_eventSubscribtion>(this);
    g_uptrSystemWindow->onSystemWindowClick.subscribe<Application, &Application::mangeWindowClickEvent>(this);

    /*
    float4x4 _matrix = {
       2, -1,  3,  3,
       0,  5,  2, -5,
       1, -1, -2,  2,
      -2,  1,  0,  1
    };

    float4x4 _identity{};

    _identity.printMatrix();
    Optim::Mathematics::getMatrixInverse(_matrix).printMatrix();
    */

    // >>>>>>>>>>> TO DO <<<<<<<<<<< 
    // 
    // Load the vertex and fragment/pixel shaders.
    // For now they are hard coded, eventually will 
    // try to implement Material system, to create 
    // shaders dynamically and give them to a mesh 
    // at runtime.

    /*
    * Lit shaders pipeline binding
    */
    SPipelineDesc pipelineDesc = {
      .vertexShaderHandle   = Graphics::RHI()->createVertexShader("bin/PhongVertexShader.cso"),
      .fragmentShaderHandle = Graphics::RHI()->createFragmentShader("bin/PhongPixelShader.cso"),

      .rasterizerDescription = {
        .fillMode = ERasterizerFillMode::Solid,
        .cullMode = ERasterizerCullMode::Back,
        .faceWinding = ERasterizerFaceWinding::CounterClockWise,
      },

      .depthStencilDescription = {
        .depthTestEnabled = true,
        .depthComparisonFunction = EDepthStencilComparisonFunction::Less,
        .depthWriteMask = EDepthStencilDepthWriteMask::WriteAll
      },

      .primitiveTopology = EPipelinePrimitiveTopology::TriangleList
    };

    _handlePipeline = Graphics::RHI()->createPipeline(&pipelineDesc);

    /*
    * Wirframe pipeline
    */
    SPipelineDesc l_wirframePipelineDesc = {
      .vertexShaderHandle   = Graphics::RHI()->createVertexShader("bin/WireframeVS.cso"),
      .fragmentShaderHandle = Graphics::RHI()->createFragmentShader("bin/WireframePS.cso"),

      .rasterizerDescription = {
        .fillMode             = ERasterizerFillMode::Wireframe,
        .cullMode             = ERasterizerCullMode::None,
        .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
        .depthBias            = -1,
        .slopeScaledDepthBias = -1.0f
      },

      .depthStencilDescription = {
        .depthTestEnabled         = true,
        .depthComparisonFunction  = EDepthStencilComparisonFunction::Less,
        .depthWriteMask           = EDepthStencilDepthWriteMask::WriteAll,
      },

      .primitiveTopology = EPipelinePrimitiveTopology::TriangleList
    };
    _handlePipelineWirframeView = Graphics::RHI()->createPipeline(&l_wirframePipelineDesc);

    /*
    * Wirframe pipeline
    */

    SPipelineDesc l_outlinePipelineDesc = {
      .vertexShaderHandle   = Graphics::RHI()->createVertexShader("bin/OutlineVS.cso"),
      .fragmentShaderHandle = Graphics::RHI()->createFragmentShader("bin/OutlinePS.cso"),

      .rasterizerDescription = {
        .fillMode             = ERasterizerFillMode::Solid,
        .cullMode             = ERasterizerCullMode::Front,
        .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
        .depthBias            = -2,
        .slopeScaledDepthBias = -1.0f
      },

      .depthStencilDescription = {
        .depthTestEnabled         = true,
        .depthComparisonFunction  = EDepthStencilComparisonFunction::Less,
        .depthWriteMask           = EDepthStencilDepthWriteMask::WriteNone,
      },

      .primitiveTopology = EPipelinePrimitiveTopology::TriangleList
    };
    _handlePipelineOutline = Graphics::RHI()->createPipeline(&l_outlinePipelineDesc);

    /* 
     * -----------------------------------------------------------------------------
     * -------------------------- Line Rendering pipeline --------------------------
     * -----------------------------------------------------------------------------
    */

    SPipelineDesc l_pipelineLineDesc = {
      .vertexShaderHandle   = Graphics::RHI()->createVertexShader("bin/WireframeVS.cso"),
      .fragmentShaderHandle = Graphics::RHI()->createFragmentShader("bin/WireframePS.cso"),

      .rasterizerDescription = {
        .fillMode             = ERasterizerFillMode::Solid,
        .cullMode             = ERasterizerCullMode::None,
        .faceWinding          = ERasterizerFaceWinding::CounterClockWise,
        .depthBias            = 0,
        .slopeScaledDepthBias = 0
      },

      .depthStencilDescription = {
        .depthTestEnabled         = true,
        .depthComparisonFunction  = EDepthStencilComparisonFunction::Less,
        .depthWriteMask           = EDepthStencilDepthWriteMask::WriteAll,
      },

      .primitiveTopology = EPipelinePrimitiveTopology::LineStrip
    };
    _handlePipelineLineRendering = Graphics::RHI()->createPipeline(&l_pipelineLineDesc);

    /* 
     * -----------------------------------------------------------------------------
     * ------------------------------ Line rendering  ------------------------------
     * -----------------------------------------------------------------------------
    */

    /*
    _meshLineRender.vertices = new Vertex[2];

    _meshLineRender.vertices[0] = Vertex {
      .position = {0, 0, 0},
      .uvCoord  = {0, 0},
      .normal   = {0, 0, 0}
    };
    _meshLineRender.vertices[1] = Vertex {
      .position = {0, 0, 10},
      .uvCoord  = {0, 0},
      .normal   = {0, 0, 0}
    };

    _meshLineRender.indices = new uint32[2]{
      0, 1
    };

    _meshLineRender.vertexCount = 2;
    _meshLineRender.indexCount = 2;

    _meshLineRender.vertexBufferHandle  = Graphics::RHI()->createResourceVertexBuffer(_meshLineRender.vertices, 2);
    _meshLineRender.indexBufferHandle   = Graphics::RHI()->createResourceIndexBuffer(_meshLineRender.indices, 2);
    */

    // Create DepthStencil state
    _handle_depthRT = Graphics::RHI()->createDepthRT();

    // Load image for texture
    Image srcImage;
    FileStream::readPngImage("images/jeff2.png", srcImage);
    _TEST_pTextureResource  = Graphics::RHI()->createTextureResource(&srcImage);

    // Create sampler resource
    _TEST_pSampler = Graphics::RHI()->createSamplerResource();

    _worldGridMesh = Mesh::createWorldGrid();

    /*
    for (int i = 0; i < 12; i++) {
      _worldGridMesh.vertices[i].print();
    }

    printf("World grid index count %d\n", _worldGridMesh.indexCount);

    float4x4 translationMatrix {
      1, 0, 0, 1,
      0, 1, 0, 2,
      0, 0, 1, 3,
      0, 0, 0, 1
    };

    //translationMatrix = translationMatrix.transpose();
    translationMatrix.printMatrix();

    printf("World grid vertex count %d\n", _worldGridMesh.vertexCount);
    */

    _worldGridMesh.vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(_worldGridMesh.vertices, _worldGridMesh.vertexCount);
    _worldGridMesh.indexBufferHandle  = Graphics::RHI()->createResourceIndexBuffer(_worldGridMesh.indices, _worldGridMesh.indexCount);

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

    Graphics::RHI()->cmdSetRenderTargets(&_handle_depthRT);
    Graphics::RHI()->cmdBindPipeline(&_handlePipeline);
    Graphics::RHI()->BindTexture(_TEST_pTextureResource);
    Graphics::RHI()->bindSampler(_TEST_pSampler);

    // >>>>>>>>>>> TO DO <<<<<<<<<<< 
    // 
    // For now application iterates through all active meshes to bind
    // their vertex and index buffers to the command buffer.
    // Eventually it will instead iterate through active objects in scene,
    // which will also allow to iterate through their materials.

    for (int i = 0; i < _list_meshes.size(); i++) {
      Graphics::RHI()->cmdBindVertexBuffer(&_list_meshes[i]->vertexBufferHandle);
      Graphics::RHI()->cmdBindIndexBuffer(&_list_meshes[i]->indexBufferHandle);
      Graphics::RHI()->cmdDrawIndexed(_list_meshes[i]->indexCount);
    }
    
    // Draw Ray casts if available
    if (_list_Rays.size() > 0) {
      Graphics::RHI()->cmdBindPipeline(&_handlePipelineLineRendering);

      for (uint64 i = 0; i < _list_Rays.size(); i++) {
        Graphics::RHI()->cmdBindVertexBuffer(&_list_Rays[i]->vertexBufferHandle);
        Graphics::RHI()->cmdBindIndexBuffer(&_list_Rays[i]->indexBufferHandle);
        Graphics::RHI()->cmdDrawIndexed(_list_Rays[i]->indexCount);
      }
    }

    // Draw wirframe for all meshes if required
    if (_bool_drawWireframe) {
      Graphics::RHI()->cmdBindPipeline(&_handlePipelineWirframeView);
      for (int i = 0; i < _list_meshes.size(); i++) {
        Graphics::RHI()->cmdBindVertexBuffer(&_list_meshes[i]->vertexBufferHandle);
        Graphics::RHI()->cmdBindIndexBuffer(&_list_meshes[i]->indexBufferHandle);
        Graphics::RHI()->cmdDrawIndexed(_list_meshes[i]->indexCount);
      }
    }

    // Draw outlines for all meshes if required
    if (_bool_drawOutline) {
		  Graphics::RHI()->cmdBindPipeline(&_handlePipelineOutline);
		  for (int i = 0; i < _list_meshes.size(); i++) {
			  Graphics::RHI()->cmdBindVertexBuffer(&_list_meshes[i]->vertexBufferHandle);
			  Graphics::RHI()->cmdBindIndexBuffer(&_list_meshes[i]->indexBufferHandle);
			  Graphics::RHI()->cmdDrawIndexed(_list_meshes[i]->indexCount);
		  }
    }

    // Execute the commands
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

  (*l_uptrMesh).vertexBufferHandle  = Graphics::RHI()->createResourceVertexBuffer(l_uptrMesh->vertices, l_uptrMesh->vertexCount);
  (*l_uptrMesh).indexBufferHandle   = Graphics::RHI()->createResourceIndexBuffer(l_uptrMesh->indices, l_uptrMesh->indexCount);

  printf("Mesh index count: %du\n", l_uptrMesh->indexCount);
  printf("Mesh tri count: %f\n", (float)l_uptrMesh->indexCount / 3);

  _list_meshes.push_back(l_uptrMesh.move());
}
