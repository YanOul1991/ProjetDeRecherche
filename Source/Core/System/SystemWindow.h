/* ======================================================================================
 *  SystemWindow.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Utilities/Event/Event.h"

#include <Core/OptimEngine.h>

/*
 * @brief
 * This class represent an OS managed window.
 */
class SystemWindow final
{
 public:
  /*
   * @brief
   * Get the window's pointer as managed by the OS.
   *
   * @return
   * Pointer to window managed by the OS as a void*. Must be cast to appropriate
   * pointer type when used in context.
   */
  static void* getSystemPointer();

  /*
   * @brief
   * SystemWindow class constructor.
   */
  SystemWindow();

  /*
   * @brief
   * SystemWindow class deconstructor.
   */
  ~SystemWindow();

  /*
   * @brief
   * Displays the system window.
   */
  void showWindow();

  /*
   * @brief
   * Initializes SDL and creates a simple window that can be resized.
   * For now the window does manage any message other than the QUIT message.
   *
   * @param windowTitle
   * The title of the window
   */
  void initialize(const char* windowTitle);

  /*
   * Runs the window loop.
   *
   * @return
   * Returns true if the window should continue to run.
   * Returns false if the window should close.
   */
  bool loop();

  /*
   * @brief
   * Set the title of the system window.
   *
   * @param title
   * The new title of the window.
   */
  void setWindowTitle(const char* title);

  /*
   * @brief
   * Destroys the current window and unitializes SDL.
   */
  void quit();

  void getWindowSize(int32* pWidth, int32* pHeight);

  bool getMouseHold();

  void getMouseDelta(float* mouseX, float* mouseY);

  /*
   * @brief
   * Window Click Event.
   * 
   * 
   */
  Delegate<float, float, int32> onSystemWindowClick{};
  Delegate<float, float, int32> onSystemWindowMouseUp{};

  /**
   * @brief
   * Window event triggered when it is resized.
   * 
   * Callback parameters:
   *  - uint32 width: New width in pixels
   *  - uint32 height: New hieght in pixels
   */
  Delegate<uint32, uint32> onWindowResize{};

  /**
   * @brief
   * User used CTRL+S action.
   */
  Delegate<> onSaveEvent{};
};