#pragma once

#include "Core/Graphics/Resource/GraphicResourceHandle.h"

enum class EPipelinePrimitiveTopologyType : unsigned char {
  TriangleList,
  LineList
};



enum class EPipelineVertexLayout : unsigned char {
  // ENUM_NOT_DEFINED_YET
};


enum class EPipelineDepthDesc : unsigned char {
  // ENUM_NOT_DEFINED_YET
};

enum class EPipelineBlendDesc : unsigned char {
  // ENUM_NOT_DEFINED_YET
};

enum class EPipelineRenderTargetFormat : unsigned char {
  // ENUM_NOT_DEFINED_YET
};

/* **************************************
    * RASTERIZER DESCIRPTIONS
************************************** */

enum class ERasterizerFillMode : unsigned char {
  Solid,
  Wireframe
};

enum class ERasterizerCullMode : unsigned char
{
  Back,
  Front,
  None
};

enum class ERasterizerFaceWinding : unsigned char
{
  ClockWise,
  CounterClockWise
};

struct SRasterizerDescription
{
  ERasterizerFillMode     fillMode;
  ERasterizerCullMode     cullMode;
  ERasterizerFaceWinding  faceWinding;
};

/* **************************************
    * DEPTH STENCIL DESCIRPTIONS
************************************** */

enum class EDepthStencilComparaisonFunction : unsigned char
{
  Never,
  Near,
  NearEqual,
  Equal,
  Far,
  FarEqual,
  NotEqual,
  Always
};

enum class EDepthStencilDepthWriteMask : unsigned char
{
  WriteAll,
  WriteNone
};

struct SDepthStencilDescription
{
  bool depthTestEnabled;
  bool depthWriteEnabled;
  EDepthStencilComparaisonFunction depthComparaisonFunction;
  EDepthStencilDepthWriteMask depthWriteMask;
};

/* **************************************
    * PIPELINE DESCIRPTION
************************************** */

struct SGraphicPipelineDesc {
  VertexShaderHandle        vertexShaderHandle;
  FragmentShaderHandle      fragmentShaderHandle;
  SRasterizerDescription    rasterizerDescription;
  SDepthStencilDescription  depthStencilDescription;
};

using PipelineHandle = GraphicResourceHandle<int>;