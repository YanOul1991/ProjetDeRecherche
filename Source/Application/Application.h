#pragma once

#ifdef DLL_LIB
#define APPLICATION_API __declspec(dllexport)
#else
#define APPLICATION_API __declspec(dllimport)
#endif

class APPLICATION_API Application final
{
public:
  Application();
  ~Application();
  void ApplicationStart();
  void ApplicationLoop();
  void ApplicationQuit();
  bool isRunning;

private:
  int loops{ 0 };
};
