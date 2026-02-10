/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      + main.cpp:
          Optim Engine system application entry point.

      + By:
          Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#include "Core/System/Application.h"
#include <iostream>

int main() {
  std::cout << "---------------- Main start ----------------\n";

  // Create application instance
  Application* application{ new Application };

  // Initialize application
  application->ApplicationStart();

  // Excecute Application running loop
  while (application->ShouldRun())
  {
    try {
      application->ApplicationLoop();
    }
    catch (const std::exception& e) {
      printf("Exception thrown: %s", e.what());
    }
  }

  // Perform cleanup
  application->ApplicationQuit();

  return 0;
}