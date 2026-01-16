/* ======================================================================================
 *  Application.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Graphics/IGraphicsModule.h"
#include "Core/Input/Input.h"
#include "Core/Time/Time.h"
#include "Core/Types/Color.h"
#include "Core/Types/string.h"
#include "Core/Exception/exception.h"
#include "Core/System/FileStream.h"
#include "Core/System/SystemWindow.h"
#include "Core/Object/Image/Image.h"
#include "Core/System/Application.h"

#include "Core/Object/Object.h"
#include "Core/Utilities/Random/Random.h"

#include <iostream>
#include <sstream>
#include <format>
#include <memory>
#include <fstream>
#include <string>

#pragma warning(disable: 4477)
#pragma warning(disable: 4313)

extern "C" {
  CORE_API Application* CreateApplicationProc()
  {
    //std::unique_ptr<Application> application = std::make_unique<Application>();
    return new Application;
  }
}

float Application::m_runtime{ 0.0f };
float Application::m_deltaTime{ 1.0f };

Application::Application() :
  m_shouldRun     { false },
  m_pRenderModule { nullptr },
  m_pInput        { nullptr },
  m_pSysWindow    { nullptr }
{ }

Application::~Application() {}

bool Application::ShouldRun() const 
{ 
  return m_shouldRun; 
}

float Application::getRuntime()
{
  return m_runtime;
}

float Application::getDeltaTime()
{
  return m_deltaTime;
}

void Application::Quit() 
{ 
  m_shouldRun = false; 
}

// Initialize apporpriate ressources when starting an application
void Application::ApplicationStart()
{
  try {
    // Start by assuming failure
    m_shouldRun = false;
    m_pSysWindow->initialize(TEXT("dvwjdvwjvdhj"));

    // Load Direct3d11 runtime module
    HMODULE hmod = LoadLibraryW(TEXT("bin/directx11_ri.dll"));

    if (hmod == nullptr) {
      THROW_EXCEPTION(TEXT("Could not load the module at \"bin/directx11_ri.dll\""));
    }

    IGraphicsModule* (*pProc)() = (IGraphicsModule* (*)())GetProcAddress(hmod, "CreateDirect3D11Module");

    if (pProc) {
      m_pRenderModule = pProc();
      m_pRenderModule->Initialize(m_pSysWindow->getSystemPointer());
    }

    /// OBJECT CLASS EXPERIMENTATION

    //String str1 = String::SPrint(TEXT("This is the first part of the string:"));
    //wprintf(TEXT("String Value: %s| String length %d"), str1.value(), str1.length());

    //wprintf(L"%s | String length %d", str1.value(), str1.length());

    //String* str = new String(TEXT("dwavdwdvu"));
    //String* str = new String();

    //printf("Created  pointer: 0x%02x\n", str);

    //String* strAlloc = new String;
    //UniquePtr<String>pStr0;
    ////UniquePtr<String>pStr1(static_cast<UniquePtr<String>&&>(pStr0));
    //UniquePtr<String>pStr1(nullptr);

    //pStr1 = pStr0.move();

    //printf("Ptr address 0 : 0x%02X\n", &(*pStr0));
    //printf("Ptr address 1 : 0x%02X\n", &(*pStr1));
    //printf("Alloc address : 0x%02X\n", strAlloc);

    //String::printf("This is a printf test function %d", 373268);

    //std::wcout << str1.value();
    //std::wcout << TEXT("This is new data\n");
    //std::cout << "ebhjfvbjhevfje";

    //String str2 = TEXT("-Hello!");
    //str1 += str2;
    //std::cout << std::boolalpha << "Same string? " << String::compare(str1, str2);
    //std::vector<Object*> objects;
    //Object* obj1 = Type::getObject(GrandChildClass::typeInfo.name);
    //Object* obj2 = Type::getObject(ChildClass::typeInfo.name);
    //Object* obj3 = Type::getObject(GrandChildClass::typeInfo.name);

    /*
    std::fstream fileStream("data/test.txt", std::ios::in | std::ios::app);

    if (!fileStream.is_open()) {
      std::cerr << "Could not open file :(\n";
    }
    else {
      std::cout << "File opened :D\n";
    }

    std::string line{};

    while (std::getline(fileStream, line)) {
      //std::cout << line.length() << '\n';
      for (unsigned char c : line) {
        std::cout << (int)c << " ";
      }

      std::cout << '\n';

      objects.push_back(Type::getObject(line.c_str()));

    }

    fileStream.close();
    */

    /*
    objects.push_back(Type::getObject(GrandChildClass::typeInfo.name));
    objects.push_back(Type::getObject(Object::typeInfo.name));
    objects.push_back(Type::getObject(ChildClass::typeInfo.name));

    for (int i = 0; i < 3; i++) {
      std::cout << "TypeInfo : " << objects[i]->getTypeInfo()->name << '\n';
    }
    */

    // std::cout << "Address  : 0x" << std::hex << obj1 << std::dec << '\n';
    // obj1->printHello();

    /// OBJECT CLASS EXPERIMENTATION - END

    m_shouldRun = true;
  }
  catch (const Exception& e) {
    String fullMessage = String(e.whatDescriptive());
    MessageBoxW(0, fullMessage.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e) {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...) {
    MessageBoxW(0, TEXT("Unknown details, for exception thrown"), TEXT("Exception..."), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
}

void Application::ApplicationLoop()
{
  try {
    static uint64 __now;
    static uint64 __last = op::time::nowHighFreq();

    if (!m_pSysWindow->loop()) {
      Quit();
      return;
    }

    if (m_pRenderModule) {
      m_pRenderModule->draw();
    }

    __now         = op::time::nowHighFreq();
    m_deltaTime   = (__now - __last) * (1000.0f / (float)op::time::getMachineFrequency()) / 1000.0f;
    __last        = __now;
    m_runtime     += m_deltaTime;
  }
  catch (const Exception& e) {
    String fullMessage = String(e.type());
    fullMessage +=  String(TEXT("\n\n[Description]\n")) + String(e.what()) + String(TEXT("\n[File]\n")) + e.getFile();

    MessageBoxW(0, fullMessage.value(), e.type(), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (const std::exception& e) {
    MessageBoxA(0, e.what(), "Error", MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
  catch (...) {
    MessageBoxW(0, TEXT("Unknown details, for exception thrown"), TEXT("Exception..."), MB_OK + MB_ICONEXCLAMATION);
    Quit();
  }
}

void Application::ApplicationQuit()
{ 
  delete(m_pRenderModule);
  delete(m_pSysWindow);
}