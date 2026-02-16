// Dx11CommandBuffer.h

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"

#include <vector>

enum ECmdBufferInstructionType {
  BindPipeline,
  BindVertexBuffer,
  BindIndexBuffer,
  BindTexture,
  SetRenderTargets,
  BindConstantBuffer,
  DrawIndexed
};

struct SCmdBufferInstruction {
  ECmdBufferInstructionType type;

  uint32 dataOffset;
};

class Dx11CommandBuffer
{
 public:
  inline void addCommand(ECmdBufferInstructionType commandType, void* pData, uint32 dataSize) {

  }

  inline void excecuteCommands() {
  
  }
};