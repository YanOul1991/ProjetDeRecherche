#pragma once
#include "d3d11.h"
#include "DirectXMath.h"
#include "d3dcompiler.h"
#include "wrl.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

template <typename T> 
using ComPtr = Microsoft::WRL::ComPtr<T>;