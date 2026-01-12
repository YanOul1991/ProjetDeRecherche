#pragma once

#include "Core/OptimEngine.h"
#include <cmath>

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

  inline ColorRgb convertColorHexToRgba(const ColorHex& _color8bit)
  {
    return{
      (((float)((_color8bit.value >> 24)  & 0xFFu)) / 255.0f) / 1.0f,
      (((float)((_color8bit.value >> 16)  & 0xFFu)) / 255.0f) / 1.0f,
      (((float)((_color8bit.value >> 8)   & 0xFFu)) / 255.0f) / 1.0f,
      (((float)((_color8bit.value)        & 0xFFu)) / 255.0f) / 1.0f
    };
  }

  inline void setHexArray(float* _arr, const ColorHex& _color8bit)
  {
    for (int i = 0; i < 4; i++)
      _arr[i] = (float)((_color8bit.value >> (24 - (8 * i))) & 0xFFu) / 255.0f / 1.0f;
  }

  inline ColorHex getColorHex(const float r, const float g, const float b, const float a)
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

  inline ColorHex getColorHex(const EColor eColor)
  {
    return{
      (uint32)eColor
    };
  }

  inline ColorHSV rgbToHsv(const ColorRgb& _color)
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
        (diff / max),
        max
      };
    }
  }

  inline void hsvToRgb(ColorHSV &_colorHsv, ColorRgb &_rgbValue)
  {
    float hue { _colorHsv.h };

    float fC{ _colorHsv.v * _colorHsv.s };
    float fX{ fC * (1 - fabsf(fmodf(hue / 60, 2.0f) - 1)) };
    float fM{ _colorHsv.v - fC };

    float rPrime{};
    float gPrime{};
    float bPrime{};

    if (hue >= 0 && hue < 60.0f)
    {
      rPrime = fC;
      gPrime = fX;
      bPrime = 0;
    }
    else if (hue >= 60.0f && hue < 120.0f)
    {
      rPrime = fX;
      gPrime = fC;
      bPrime = 0;
    }
    else if (hue >= 120.0f && hue < 180.0f)
    {
      rPrime = 0;
      gPrime = fC;
      bPrime = fX;
    }
    else if (hue >= 180.0f && hue < 240.0f)
    {
      rPrime = 0;
      gPrime = fX;
      bPrime = fC;
    }
    else if (hue >= 240.0f && hue < 300.0f)
    {
      rPrime = fX;
      gPrime = 0;
      bPrime = fC;
    }
    else if (hue >= 300.0f && hue < 360.0f)
    {
      rPrime = fC;
      gPrime = 0;
      bPrime = fX;
    }
    
    _rgbValue.r = rPrime + fM;
    _rgbValue.g = gPrime + fM;
    _rgbValue.b = bPrime + fM;
  }

  inline ColorRgb hsvToRgb(ColorHSV& _colorHsv)
  {
    float hue{ _colorHsv.h };

    float fC{ _colorHsv.v * _colorHsv.s };
    float fX{ fC * (1 - fabsf(fmodf(hue / 60, 2.0f) - 1)) };
    float fM{ _colorHsv.v - fC };

    float rPrime{};
    float gPrime{};
    float bPrime{};

    if (hue >= 0 && hue < 60.0f)
    {
      rPrime = fC;
      gPrime = fX;
      bPrime = 0;
    }
    else if (hue >= 60.0f && hue < 120.0f)
    {
      rPrime = fX;
      gPrime = fC;
      bPrime = 0;
    }
    else if (hue >= 120.0f && hue < 180.0f)
    {
      rPrime = 0;
      gPrime = fC;
      bPrime = fX;
    }
    else if (hue >= 180.0f && hue < 240.0f)
    {
      rPrime = 0;
      gPrime = fX;
      bPrime = fC;
    }
    else if (hue >= 240.0f && hue < 300.0f)
    {
      rPrime = fX;
      gPrime = 0;
      bPrime = fC;
    }
    else if (hue >= 300.0f && hue < 360.0f)
    {
      rPrime = fC;
      gPrime = 0;
      bPrime = fX;
    }

    return {
      rPrime + fM,
      gPrime + fM,
      bPrime + fM,
      1.0f
    };
  }
};
