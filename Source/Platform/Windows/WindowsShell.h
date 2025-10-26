#pragma once

#ifdef DLL_LIB
#define WIN_SHELL_API __declspec(dllexport)
#else
#define WIN_SHELL_API __declspec(dllimport)
#endif

#include "Global/OptimEngineGlobal.h"
#include "Global/WindowsSettings.h"

namespace op::win::shell
{
  HRESULT WIN_SHELL_API createDesktopShortcut(
    const std::wstring& _filePath,
    const std::wstring& _shortcutPath,
    const std::wstring& _description);

  HRESULT WIN_SHELL_API getFilePath(std::wstring* _str);
  HRESULT WIN_SHELL_API getDirPath(std::wstring* _str);
}
