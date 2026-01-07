/* ======================================================================================
 *  msDx11.h:
 *		This file contains directives, macros and other global declarations for
 *		using the directx11 API.
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define OPTIM_TRY_DX(_PROC_) if(FAILED( hr = _PROC_)) throw Exception(__LINE__, __FILEW__, hr, TEXT("DirectX Error"), op::sys::windows::translateError(hr))