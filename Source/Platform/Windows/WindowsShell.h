#pragma once

#include "Global/OptimEngineGlobal.h"
#include "WindowsShellTools.h"

namespace OShell
{
  HRESULT createDesktopShortcut(
    const std::wstring& _filePath, 
    const std::wstring& _shortcutPath, 
    const std::wstring& _description);

  HRESULT getFilePath(std::wstring* _str);
  HRESULT getDirPath(std::wstring* _str);
}