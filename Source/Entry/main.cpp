#include "Global/OptimEngineGlobal.h"
#include "Application/Application.h"

int main() {
#if defined(WINDOWS_OS)
  AllocConsole();
  FILE* fp;
  freopen_s(&fp, "CONOUT$", "w", stdout);
  freopen_s(&fp, "CONOUT$", "w", stderr);
#endif

  std::cout << "Hello, Console!\n";

  Application application{};

  application.ApplicationStart();

  while (application.ShouldRun())
  {
    application.ApplicationLoop();
  }
  application.ApplicationQuit();

  return 0;
}