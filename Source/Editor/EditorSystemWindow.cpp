#include "Editor/EditorSystemWindow.h"

// Static definitions
EditorSystemWindow* EditorSystemWindow::sm_pSingleton{ nullptr };

EditorSystemWindow::EditorSystemWindow() { }
EditorSystemWindow::~EditorSystemWindow() { }

EditorSystemWindow* EditorSystemWindow::CreateEditorSystemWindow()
{
	if (sm_pSingleton == nullptr)
	{
		sm_pSingleton = new EditorSystemWindow;
		return sm_pSingleton;
	}
	else
	{
		MessageBoxA(0, "A BaseSystemWindow Class already exists.", "BaseSystemWindow Class Debug", MB_OK + MB_ICONASTERISK);
		return nullptr;
	}
}
