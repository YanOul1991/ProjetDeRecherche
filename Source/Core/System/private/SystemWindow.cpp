/* ======================================================================================
 *  SystemWindow.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#define SDL_MAIN_HANDLED // program main function NOT managed by SDL

// SDL lib dependencies
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "version.lib")

//#include "Core/Defines/Windows/windowsAPI.h"
#include "Core/Object/Camera/Camera.h"
#include "Core/Math/OptimMathematics.h"
#include "Core/Math/Quaternion.h"

#include "ThirdParty/SDL3/SDL.h"
#include "ThirdParty/SDL3/SDL_system.h"

#include "../SystemWindow.h"

#include <iostream>
#include <iomanip>
#include <cstdio>

#include "Core/_Temporary/InterfaceImGui.h"
#include "ThirdParty/imgui/imgui_impl_sdl3.h"

static SDL_Window* window;
static bool shouldRun = true;

SystemWindow::SystemWindow(){ }
SystemWindow::~SystemWindow() { }

void SystemWindow::initialize(const wchar* windowTitle)
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Optim Engine - SDL - DirectX 11", 1280, 720, SDL_WINDOW_RESIZABLE);

	if (!SDL_MaximizeWindow(window)) {
		printf("Failed to maximize window.");
	}

	//SDL_SetWindowRelativeMouseMode(window, false);

	InterfaceImGui::initWindow(window);

	// Camera initial data.

	Camera::forward = Camera::rotation.rotate({0.0f, 0.0f, 1.0f});
	Camera::right		= Camera::rotation.rotate({1.0f, 0.0f, 0.0f});
	Camera::up			= Camera::rotation.rotate({0.0f, 1.0f, 0.0f});

	Camera::position = { 0.0f, 0.0f, -5.0f };
}

bool SystemWindow::loop() 
{
	static bool	 wheelHold = false;
	static bool	 leftBtnMouse = false;
	static float mouseDx;
	static float mouseDy;

	SDL_Event							evt{};
	SDL_MouseButtonFlags	mouseData			= SDL_GetMouseState(0, 0);
	const bool*						keyboardState = SDL_GetKeyboardState(0);

	wheelHold			= mouseData & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE);
	leftBtnMouse	= mouseData & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);

	SDL_GetRelativeMouseState(&mouseDx, &mouseDy);

	while (SDL_PollEvent(&evt)) {
		ImGui_ImplSDL3_ProcessEvent(&evt);
		if (evt.type == SDL_EVENT_QUIT) {
			shouldRun = false;
			break;
		}
		if (evt.type == SDL_EVENT_MOUSE_WHEEL) {
			Camera::position = Camera::position + evt.wheel.y * 0.5f * Camera::forward;
			break;
		}
	}// While end - Event poll loop

	/*
	 * Viewport camera management.
	 * When holding the middle mouse button camera move left and right.
	 * If also holding the left shift key then camera rotations on Y 
	 * axis and X axis
	*/
	if (wheelHold) {
		if (keyboardState[SDL_SCANCODE_LSHIFT]) {
			Camera::position = Camera::position + (mouseDx * 0.01f * Camera::right);
			Camera::position = Camera::position + (mouseDy * 0.01f * Camera::up);
		}
		else {
			Quaternion qYaw = Quaternion::fromAxisAngle({0.0f, 1.0f, 0.0f}, mouseDx * 0.005f);
			Camera::rotation = qYaw * Camera::rotation;

			Camera::right = Camera::rotation.rotate({1.0f, 0.0f, 0.0f});
			Quaternion qPitch = Quaternion::fromAxisAngle(Camera::right, mouseDy * 0.005f);
			Camera::rotation = qPitch * Camera::rotation;

			Camera::forward = Camera::rotation.rotate({0.0f, 0.0f, 1.0f});
			Camera::right = Camera::rotation.rotate({1.0f, 0.0f, 0.0f});
			Camera::up = Camera::rotation.rotate({0.0f, 1.0f, 0.0f});
		}
	}
	return shouldRun;
}

void SystemWindow::setWindowTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void* SystemWindow::getSystemPointer()
{
	SDL_PropertiesID props = SDL_GetWindowProperties(window);
	return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, 0);
}

void SystemWindow::quit()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}