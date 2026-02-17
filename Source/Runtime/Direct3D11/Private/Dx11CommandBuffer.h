// Dx11CommandBuffer.h

#pragma once

#include "Core/Defines/DirectX/msDx11.h"
#include "Core/Defines/Windows/windowsAPI.h"

#include <vector>

enum class ECommandType {
  BindPipeline,
  BindVertexBuffer,
  BindIndexBuffer,
  BindTexture,
  DrawIndexed,
  SetRenderTargets,
  BindConstantBuffer,
  BindConstantBufferTransformMatrix
};

struct SCommand {
  ECommandType type;
  uint32       dataOffset;
};

class CommandBuffer final
{
 public:
  /*
   * @brief
   * Add a command to the command buffer.
   *
   * @param param_cmd_Type
   * The command type from the ECommandType class enum.
   *
   * @param param_pData
   * A pointer to the required data to bind the resource, these will be stored as raw bytes.
   * 
   * @param the byte size if the data passed in the
   *
   */
  void push(ECommandType param_cmdType, void* param_pData, uint32 param_dataSize) {
    // Create a new command structure
    // Assign the type of the data and
    // the offset of the data to be placed
    // inside the data buffer.

    SCommand cmd{};
    cmd.type       = param_cmdType;
    cmd.dataOffset = (uint32)data.size();

    commands.push_back(cmd);
    data.insert(data.end(), reinterpret_cast<uint8*>(param_pData), reinterpret_cast<uint8*>(param_pData) + param_dataSize);
  }

  std::vector<SCommand> commands;
  std::vector<uint8>    data;
};