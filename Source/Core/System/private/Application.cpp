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


/*
* STATIC GLOBAL VARIABLES 
* 
* >>>> THESE ARE FOR TESTING PURPOSES <<<<
* 
*/

static ITextureResource*    _TEST_pTextureResource  {};
static ISampler*            _TEST_pSampler          {};

static std::vector<UniquePtr<Mesh>> _list_meshes{};
static UniquePtr<SystemWindow>      g_uptrSystemWindow{};

static Mesh _worldGridMesh{};

static PipelineHandle         _handlePipeline{};
static PipelineHandle         _handlePipelineWirframeView{};
static PipelineHandle         _handlePipelineOutline{};

static DepthRTHandle          _handle_depthRT{};
static VertexShaderHandle     _handle_vertexShader{};
static FragmentShaderHandle   _handle_fragmentShader{};

static bool _bool_drawWireframe{false};

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

      .primitiveTopology = EPipelinePrimitiveTopology::LineList
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

    // Create DepthStencil state
    _handle_depthRT = Graphics::RHI()->createDepthRT();

    // Load image for texture
    Image srcImage;
    FileStream::readPngImage("images/jeff2.png", srcImage);
    _TEST_pTextureResource  = Graphics::RHI()->createTextureResource(&srcImage);

    // Create sampler resource
    _TEST_pSampler = Graphics::RHI()->createSamplerResource();

    _worldGridMesh = Mesh::createWorldGrid();

    //for (int i = 0; i < 12; i++) {
    //  _worldGridMesh.vertices[i].print();
    //}

    //printf("World grid index count %d\n", _worldGridMesh.indexCount);

    //printf("World grid vertex count %d\n", _worldGridMesh.vertexCount);

    _worldGridMesh.vertexBufferHandle = Graphics::RHI()->createResourceVertexBuffer(_worldGridMesh.vertices, _worldGridMesh.vertexCount);
    _worldGridMesh.indexBufferHandle = Graphics::RHI()->createResourceIndexBuffer(_worldGridMesh.indices, _worldGridMesh.indexCount);

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
    //Graphics::RHI()->cmdBindVertexShader(&_handle_vertexShader);
    //Graphics::RHI()->cmdBindFragmentShader(&_handle_fragmentShader);

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

    Graphics::RHI()->cmdBindPipeline(&_handlePipelineWirframeView);
    Graphics::RHI()->cmdBindVertexBuffer(&_worldGridMesh.vertexBufferHandle);
    Graphics::RHI()->cmdBindIndexBuffer(&_worldGridMesh.indexBufferHandle);
    Graphics::RHI()->cmdDrawIndexed(_worldGridMesh.indexCount);

    /*
    for (int i = 0; i < _list_meshes.size(); i++) {
      Graphics::RHI()->cmdBindVertexBuffer(&_list_meshes[i]->vertexBufferHandle);
      Graphics::RHI()->cmdBindIndexBuffer(&_list_meshes[i]->indexBufferHandle);
      Graphics::RHI()->cmdDrawIndexed(_list_meshes[i]->indexCount);
    }

		Graphics::RHI()->cmdBindPipeline(&_handlePipelineOutline);
		for (int i = 0; i < _list_meshes.size(); i++) {
			Graphics::RHI()->cmdBindVertexBuffer(&_list_meshes[i]->vertexBufferHandle);
			Graphics::RHI()->cmdBindIndexBuffer(&_list_meshes[i]->indexBufferHandle);
			Graphics::RHI()->cmdDrawIndexed(_list_meshes[i]->indexCount);
		}
    */

    // Execute the commands afters binding all the appropriate ones.
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

  _list_meshes.push_back(l_uptrMesh.move());
}
