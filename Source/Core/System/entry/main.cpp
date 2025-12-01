/* ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

      ++ main.cpp :
            Optim Engine application entry procedure

      ++ By:
            Yanis Oulmane

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; */

#include "Core/System/Application.h"

int main() {
  // Load necessary libraries
  Application* (*applicationProc)() = op::system::LOAD_LIB_PROC<PROC_PTR(Application)>("Core.dll", "CreateApplicationProc");

  if (!applicationProc) return 1;
  
  // Create application instance
  Application* application{ applicationProc() };

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