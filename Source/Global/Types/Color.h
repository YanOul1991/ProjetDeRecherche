#pragma once

#include "OptimEngineTypeLayers.h"

namespace op::color
{ 
  enum class EChannel : uint32
  {
    red     = 0xFF000000u,
    green   = 0x00FF0000u,
    blue    = 0x0000FF00u,
    alpha   = 0x000000FFu
  };

  enum class EColor : uint32
  {
    white = 0xFFFFFFFFu,
    black = 0x000000FFu,
    red   = 0xFF0000FFu,
    green = 0x00FF00FFu,
    blue  = 0x0000FFFFu
  };

  struct ColorRgb
  {
    float r;
    float g;
    float b;
    float a;
  };

  struct ColorHSV
  {
    float h;
    float s;
    float v;
  };

  struct ColorHex
  {
    uint32 value = static_cast<uint32>(EColor::black);
  };

  inline ColorRgb ConvertColorHexToRgba(const ColorHex& _color8bit)
  {
    return {
      (float)(((_color8bit.value & 0xFF000000u) >> 24) / 255.0f) / 1.0f,
      (float)(((_color8bit.value & 0x00FF0000u) >> 16) / 255.0f) / 1.0f,
      (float)(((_color8bit.value & 0x0000FF00u) >> 8)  / 255.0f) / 1.0f,
      (float)(((_color8bit.value & 0x000000FFu) >> 0)  / 255.0f) / 1.0f
    };

    //return{
    //  (((float)((_color8bit.value >> 24)  & 0xFFu)) / 255.0f) / 1.0f,
    //  (((float)((_color8bit.value >> 16)  & 0xFFu)) / 255.0f) / 1.0f,
    //  (((float)((_color8bit.value >> 8)   & 0xFFu)) / 255.0f) / 1.0f,
    //  (((float)((_color8bit.value)        & 0xFFu)) / 255.0f) / 1.0f
    //};
  }

  inline void SetHexArray(float* _arr, const ColorHex& _color8bit)
  {
    _arr[0] = (float)(((_color8bit.value & 0xFF000000u) >> 24) / 255.0f) / 1.0f;
    _arr[1] = (float)(((_color8bit.value & 0x00FF0000u) >> 16) / 255.0f) / 1.0f;
    _arr[2] = (float)(((_color8bit.value & 0x0000FF00u) >> 8)  / 255.0f) / 1.0f;
    _arr[3] = (float)(((_color8bit.value & 0x000000FFu) >> 0)  / 255.0f) / 1.0f;

    // for (int i = 0; i < 4; i++) 
    //   _arr[i] = (((float)((_color8bit.value >> (8 * i)) & 0xFFu)) / 255.0f) / 1.0f;
  }

  inline ColorHex GetColorHex(const float r, const float g, const float b, const float a)
  {
    return{
      (uint32)( 
        ((uint8)(r * 255u)) << 24 |
        ((uint8)(g * 255u)) << 16 |
        ((uint8)(b * 255u)) << 8  |
        ((uint8)(a * 255u)) << 0
        )
    };
  }

  inline ColorHex GetColorHex(const EColor eColor)
  {
    return{
      (uint32)eColor
    };
  }

  inline ColorHSV RgbToHsv(const ColorRgb& _color)
  {
    float channels[]{ _color.r, _color.g, _color.b };
    float max{ 0.0f };
    float min{ 1.0f };
    float diff{ 0 };

    for (int i = 0; i < 3; i++)
    {
      if (channels[i] > max)
      {
        max = channels[i];
        continue;
      }

      if (channels[i] < min)
      {
        min = channels[i];
        continue;
      }
    }

    diff = max - min;

    if (max == 0.0f)
    {
      return{ 0, 0, 0 };
    }
    else
    {
      return{
        max == _color.r ? 60 * ((int)((_color.g - _color.b) / diff) % 6) : 
        max == _color.g ? 60 * (((_color.b - _color.r) / diff) + 2) :
        max == _color.b ? 60 * (((_color.r - _color.g) / diff) + 4) : 0,
        (diff / max) * 100,
        max * 100
      };
    }
  }
};
