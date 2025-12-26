/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + main.cpp:
          Optim Engine system application entry point.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#include "Core/System/Application.h"

int main() {
  // Create application instance
  Application* application{ new Application };

  // Initialize application
  application->ApplicationStart();

  // Excecute Application running loop
  while (application->ShouldRun())
  {
    application->ApplicationLoop();
  }

  // Perform cleanup
  application->ApplicationQuit();

  return 0;
}