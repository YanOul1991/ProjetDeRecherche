/* ======================================================================================
 *  WindowsProc.h:
 *		Declaration for windows OS specific functions
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/Defines/Windows/windowsAPI.h"

namespace op::sys::windows
{
	bool messageLoop();
	LPARAM __stdcall windowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void initRawInput(void* pWindow);
}