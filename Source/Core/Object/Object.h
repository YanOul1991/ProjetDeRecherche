#pragma once

#ifdef DLL_LIB
#define OBJECT_API __declspec(dllexport)
#else
#define OBJECT_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"
#include "Random/Random.h"
#include <iostream>
#include <unordered_map>

class Component;

class OBJECT_API Object
{
public:
  static std::unique_ptr<Object> instanciate(const Object&& object);
  static void destroy(Object& object);

  Object();
  ~Object();
  std::string id() const;
  guid_t getInstanceID() const;

private:
  void update();
  const guid_t m_guid;
  static std::unordered_map<guid_t, Object*> sm_registery;
};