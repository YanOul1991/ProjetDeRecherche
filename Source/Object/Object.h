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

struct guid_t;
class MultiEvent; 
class MainWindow;

class OBJECT_API Object
{
public:
  static std::unique_ptr<Object> instanciate(const Object&& object);
  static void destroy(Object& object);

  Object(MainWindow& mainWindow);
  ~Object();
  std::string id() const;
  guid_t getInstanceID() const;
  void test();

private:
  static std::unordered_map<guid_t, Object*> sm_registery;

  void update();
  const guid_t m_guid;
  const uint32_t m_id = 11;
  MainWindow& pMainWindow;
};