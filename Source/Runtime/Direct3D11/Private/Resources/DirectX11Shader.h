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

#include "Private/Resources/Shader/DirectX11VertexShader.h"
#include "Private/Resources/Shader/DirectX11PixelShader.h"

/*
struct VertexShader {
	const wchar* path;
	ComPtr<ID3D11VertexShader> pShader;
	ComPtr<ID3D11InputLayout>  pInputLayout;
};

struct PixelShader {
	const wchar* path;
	ComPtr<ID3D11PixelShader> pShader;
};

class GFXMaterial
{
public:
	inline GFXMaterial() = default;
	inline ~GFXMaterial() = default;

	inline void setPath(const wchar* vertexShaderPath, const wchar* pixelShaderPath) {
		vertexShader.path = vertexShaderPath;
		pixelShader.path	= pixelShaderPath;
	}

	inline void loadShaders(ID3D11Device* device)
	{
		ComPtr<ID3DBlob> pBlob;

		D3DReadFileToBlob(pixelShader.path, &pBlob);
		device->CreatePixelShader(
			pBlob->GetBufferPointer(), 
			pBlob->GetBufferSize(),
			nullptr,
			&pixelShader.pShader
		);

		// Setting vertex shader

		D3DReadFileToBlob(vertexShader.path, &pBlob);
		device->CreateVertexShader(
			pBlob->GetBufferPointer(), 
			pBlob->GetBufferSize(),
			nullptr,
			&vertexShader.pShader
		);

		// Input
		const D3D11_INPUT_ELEMENT_DESC ied[] = {
			{"POSITION" , 0,  DXGI_FORMAT_R32G32B32_FLOAT,  0, 0,   D3D11_INPUT_PER_VERTEX_DATA,  0 },
			{"TEXCOORD" , 0,  DXGI_FORMAT_R32G32_FLOAT,     0, 12,  D3D11_INPUT_PER_VERTEX_DATA,  0 }
		};

		device->CreateInputLayout(
			ied,
			std::size(ied),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&vertexShader.pInputLayout
		);
	}

	VertexShader vertexShader;
	PixelShader  pixelShader;
};
*/

/*
 * @brief
 * Namespace for testing features.
*/
namespace Optim::Test {

/*
 * @brief
 * New Material Logic testing class
*/
class Material final 
{
public:
	inline Material() = default;
	inline ~Material(){};

	/*
	 * @brief 
	 * Initalizes the vertex shader and the pixel shder and allocates resources.
	 * 
	 * @param pDevice
	 * > A pointer to a valid ID3D11Device.
	 * 
	 * @param vertexShaderPath
	 * > The relative path of the cso file containing the vertex compiled shader data.
	 * 
	 * @param pixelShaderPath
	 * > The relative path of the cso file containing the compiled pixel shader data.
	*/
	inline void initializeMaterial(ID3D11Device* pDevice, const wchar* vertexShaderPath, const wchar* pixelShaderPath) {
		vertexShader.path = vertexShaderPath;
		pixelShader.path	= pixelShaderPath;

		vertexShader.init(pDevice);
		pixelShader.init(pDevice);
	}

	/*
	 * @brief
	 * Bind the shaders in the rendering pipeline.
	*/
	inline void bindShaders(ID3D11DeviceContext* pContext) {
		vertexShader.bind(pContext);
		pixelShader.bind(pContext);
	}

	DirectX11VertexShader vertexShader{};
	DirectX11PixelShader	pixelShader{};
};

} // namespace Optim::Test - END
