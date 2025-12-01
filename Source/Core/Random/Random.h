#pragma once

#include "Global/OptimEngineGlobal.h"
#include <iostream>

#ifdef DLL_LIB
  #define RANDOM_API __declspec(dllexport)
#else
  #define RANDOM_API __declspec(dllimport)
#endif

struct RANDOM_API guid_t
{
  uint64_t high;
  uint64_t low;
};

// Make guid_t hashable
template<>
struct std::hash<guid_t>
{
  std::size_t operator()(const guid_t& _guid) const
  {
    return std::hash<uint64_t>()(_guid.high) ^ (std::hash<uint64_t>()(_guid.low) << 1);
  }
};

//  Compares two GUIDs value.
bool RANDOM_API operator==(const guid_t& left, const guid_t& right);

// Get a randomly generated GUID
guid_t RANDOM_API get_random_id();

// Convert a GUID into a readable string
std::string RANDOM_API get_guid_str(const guid_t& _guid);
