#pragma once

#include "Core/Graphics/Resource/GraphicResourceHandle.h"

enum class EPipelinePrimitiveTopology : unsigned char {
  Undefined,
  PointList,
  LineList,
  LineStrip,
  TriangleList,
  TriangleStrip,
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
  Wireframe,
  Solid,
};

enum class ERasterizerCullMode : unsigned char
{
  None,
  Front,
  Back
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
  int32                   depthBias;
  float                   slopeScaledDepthBias;
};

/* **************************************
    * DEPTH STENCIL DESCIRPTIONS
************************************** */

enum class EDepthStencilComparisonFunction : unsigned char
{
  Never,
  Less,
  Equal,
  LessOrEqual,
  Greater,
  NotEqual,
  GreaterOrEqual,
  Always
};

enum class EDepthStencilDepthWriteMask : unsigned char
{
  WriteNone,
  WriteAll
};

struct SDepthStencilDescription
{
  bool depthTestEnabled;
  EDepthStencilComparisonFunction depthComparisonFunction;
  EDepthStencilDepthWriteMask     depthWriteMask;
};

/* **************************************
    * PIPELINE DESCIRPTION
************************************** */

struct SPipelineDesc 
{
  VertexShaderHandle          vertexShaderHandle;
  FragmentShaderHandle        fragmentShaderHandle;
  SRasterizerDescription      rasterizerDescription;
  SDepthStencilDescription    depthStencilDescription;
  EPipelinePrimitiveTopology  primitiveTopology;
};