#pragma once

#ifdef DLL_LIB
#define EDITOR_WINDOW_API __declspec(dllexport)
#else
#define EDITOR_WINDOW_API __declspec(dllimport)
#endif

#include "IWindow.h"

class EDITOR_WINDOW_API EditorSystemWindow final : public IWindow
{
public:
  static EditorSystemWindow* CreateEditorSystemWindow();
  ~EditorSystemWindow();

private:
  static EditorSystemWindow* sm_pSingleton;
  EditorSystemWindow();
  EditorSystemWindow(const EditorSystemWindow&) = delete;
  EditorSystemWindow(const EditorSystemWindow&&) = delete;
  EditorSystemWindow& operator=(const EditorSystemWindow&) = delete;
  EditorSystemWindow& operator=(const EditorSystemWindow&&) = delete;
};