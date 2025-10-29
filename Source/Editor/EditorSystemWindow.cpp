#include "Editor/EditorSystemWindow.h"

// Static definitions
EditorSystemWindow* EditorSystemWindow::sm_pSingleton{ nullptr };

EditorSystemWindow::EditorSystemWindow() { }
EditorSystemWindow::~EditorSystemWindow() { }

EditorSystemWindow* EditorSystemWindow::CreateEditorSystemWindow(Application* _pApplication)
{
	if (sm_pSingleton == nullptr)
	{
		sm_pSingleton = new EditorSystemWindow;
		sm_pSingleton->m_pApplication = _pApplication;
		return sm_pSingleton;
	}
	else
	{
		return nullptr;
	}
}
