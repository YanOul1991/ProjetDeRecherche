#include "Global/OptimEngineGlobal.h"
#include "Application/Application.h"

int main() {
#if defined(OPTIMENGINE_EDITOR_CONSOLE_DEBUG)

#if defined(WINDOWS_OS)

  AllocConsole();
  FILE* fp;
  freopen_s(&fp, "CONOUT$", "w", stdout);
  freopen_s(&fp, "CONOUT$", "w", stderr);
#endif

#endif

  std::cout << "***************** Console Debug Started *****************\n\n";

  // Create Application instance
  Application application{};

  // Initialize application
  application.ApplicationStart();

  // Excecute Application running loop
  while (application.ShouldRun())
  {
    application.ApplicationLoop();
  }

  // Perform cleanup
  application.ApplicationQuit();

  return 0;
}