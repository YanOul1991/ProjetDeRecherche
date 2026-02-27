/*
  Application.h:

  This header file constains the declaration of the Application class.

  The Application class serves as a central module that managed lifetimes of other Engine
  implemented modules, and manages the life time of the application.
*/

#pragma once

#include "Core/OptimEngine.h"

#include <vector>

class CORE_API Application final
{
 public:
  Application();
  ~Application();
  void ApplicationStart();
  void ApplicationLoop();
  void ApplicationQuit();
  void Quit();

  /**
   * \brief
   * Checks if the program should continue to run its loop
   * logic or if it should stop.
   *
   * \return
   * True if should continue looping, false if should stop.
   */
  bool ShouldRun() const;

  /**
   * \brief
   * Get the size of the main window.
   *
   * \param pWidth
   * A pointer to an int32 to hold the width value.
   *
   * \param pHeight
   * A pointer to an int32 to hold the height value.
   */
  static void getMainWindowSize(int32* pWidth, int32* pHeight);

 private:
  bool m_shouldRun{ false }; // Should the program continue to run or exit.

  /**
   * \brief
   * Function that will subscribed to the SystemWindow's
   * onKeyDownEvent.
   *
   * \param keycode
   * The keycode of the pressed keyboard key.
   */
  void manageKeyDownEvent(uint32 keycode);

  /**
   * \brief
   * Function that will subscribed to the SystemWindow's
   * onClickEvent.
   *
   * \param posX
   * The relative x position where the click occured.
   *
   * \param posY
   * The relative y position where the click occured.
   *
   * \param buttonID
   * The ID of the mouse button that has been clicked.
   */
  void mangeWindowClickEvent(float posX, float posY, int32 buttonID);

  /**
   * \brief
   * Function that will subscribed to the SystemWindow's
   * onMouseUpEvent.
   *
   * \param posX
   * The relative x position where the mouse release occured.
   *
   * \param posY
   * The relative y position where the mouse release occured.
   *
   * \param buttonID
   * The ID of the mouse button that has been released.
   */
  void manageSysWinMouseUp(float posX, float posY, int32 buttonID);

  /**
   * \brief
   * Function that will subscribed to the SystemWindow's
   * onWindowResizeEvent.
   *
   * \param width
   * The new width of the window.
   *
   * \param height
   * The new height of the window.
   */
  void manageWindowResizeEvent(uint32 width, uint32 height);

  /**
   * \brief
   * Function that will subscribed to the SystemWindow's
   * onSaveEvent (when a user inputs CTRL+S).
   */
  void manageOnSaveEvent();

  /**
   * \brief
   * Function that will subscribe to the SystemWindow's
   * onFileDroppedDelegate.
   *
   * \param path
   * The absolute path of the dropped file.
   *
   * \param posX
   * The releative x position of the mouse where the file was dropped.
   *
   * \param posY
   * The releative y position of the mouse where the file was dropped.
   */
  void manageOnFileDropped(const char* path, float posX, float posY);
};

extern "C" CORE_API Application* CreateApplicationProc();