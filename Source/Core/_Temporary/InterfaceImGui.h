/* ======================================================================================
 *  InterfaceImGui.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#pragma once

#include "Core/OptimEngine.h"

class CORE_API InterfaceImGui
{
public:
	InterfaceImGui();
	~InterfaceImGui();
	static void initWindow(void* sdlWindow);
	static void initDirectX(void* pDevice, void* pContext);
	static void update();
};