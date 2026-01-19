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
