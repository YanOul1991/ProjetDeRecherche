#pragma once

#include "Core/Graphics/Resource/GraphicResourceHandle.h"

#include <vector>

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

enum class ERasterizerCullMode : unsigned char {
  None,
  Front,
  Back
};

enum class ERasterizerFaceWinding : unsigned char {
  ClockWise,
  CounterClockWise
};

struct SRasterizerDescription {
  ERasterizerFillMode    fillMode;
  ERasterizerCullMode    cullMode;
  ERasterizerFaceWinding faceWinding;
  int32                  depthBias;
  float                  slopeScaledDepthBias;
};

/* **************************************
 * DEPTH STENCIL DESCIRPTIONS
 ************************************** */

enum class EDepthStencilComparisonFunction : unsigned char {
  Never,
  Less,
  Equal,
  LessOrEqual,
  Greater,
  NotEqual,
  GreaterOrEqual,
  Always
};

enum class EDepthStencilDepthWriteMask : unsigned char {
  WriteNone,
  WriteAll
};

struct SDepthStencilDescription {
  bool                            depthTestEnabled;
  EDepthStencilComparisonFunction depthComparisonFunction;
  EDepthStencilDepthWriteMask     depthWriteMask;
};

enum class EGraphicsFormat {
  Unkown,

  r32g32b32a32_typeless,
  r32g32b32a32_float,
  r32g32b32a32_uint,
  r32g32b32a32_sint,

  r32g32b32_typeless,
  r32g32b32_float,
  r32g32b32_uint,
  r32g32b32_sint,

  r32g32_typeless,
  r32g32_float,
  r32g32_uint,
  r32g32_sint,
};

enum class EInputUsageSlot {
  position  = 0,
  textCoord = 1,
  normal    = 2,
  tangent   = 3,
  color     = 4,
};

struct SPipelineInputDescription {
  const char*     name;
  EGraphicsFormat format;
  uint32          inputSlot;
  EInputUsageSlot inputUsage;
};

struct SPipelineDesc {
  const char*                            vertexShader;
  const char*                            fragmentShader;
  SRasterizerDescription                 rasterizerDescription;
  SDepthStencilDescription               depthStencilDescription;
  EPipelinePrimitiveTopology             primitiveTopology;
  std::vector<SPipelineInputDescription> inputs;
};