#pragma once

#if defined(__INTELLISENSE__) || !defined(USE_CPP20_MODULES)
#include <vulkan/vulkan_raii.hpp>
#else
import vulkan_hpp;
#endif

#ifdef DLL_LIB
#define OPT_VULKAN_API __declspec(dllexport)
#else
#define OPT_VULKAN_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"

class OPT_VULKAN_API GraphicsVulkan
{
public:
  void CreateContext();

private:
  vk::raii::Context context;
  vk::raii::Instance instance{ nullptr };
};
