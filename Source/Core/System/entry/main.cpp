// main.cpp

#include "Core/System/Application.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

int main() {
  // Create application instance
  Application* application{new Application};

  // Initialize application
  application->ApplicationStart();

  // Excecute Application running loop
  while (application->ShouldRun()) {
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