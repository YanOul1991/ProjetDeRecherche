/* ======================================================================================
 *  DirectX11Shader.h:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"
#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Defines/DirectX/msDx11.h"

struct VertexShader
{
	const wchar* path;
	ComPtr<ID3D11VertexShader> pShader;
	ComPtr<ID3D11InputLayout>  pInputLayout;
};

struct PixelShader
{
	const wchar* path;
	ComPtr<ID3D11PixelShader> pShader;
};

class GFXMaterial
{
public:
	inline GFXMaterial() = default;
	inline ~GFXMaterial() = default;

	inline void setPath(const wchar* vertexShaderPath, const wchar* pixelShaderPath)
	{
		vertexShader.path = vertexShaderPath;
		pixelShader.path	= pixelShaderPath;
	}

	VertexShader vertexShader;
	PixelShader  pixelShader;
};
