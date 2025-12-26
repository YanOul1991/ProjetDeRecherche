/* ======================================================================================
 *   OpDirect3d11Global.h:
 *    Global setting for all files using D3D11 API.
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once
#include "Core/Defines/Windows/windowsAPI.h"

#include "d3d11.h"
#include "DirectXMath.h"
#include "d3dcompiler.h"
#include "wrl.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")