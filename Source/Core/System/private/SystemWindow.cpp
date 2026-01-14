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

	SDL_SetWindowRelativeMouseMode(window, false);

	InterfaceImGui::initWindow(window);

	// Camera initial data.

	Camera::Forward = Camera::rotation.rotate({0.0f, 0.0f, 1.0f});
	Camera::right = Camera::rotation.rotate({1.0f, 0.0f, 0.0f});
	Camera::up = Camera::rotation.rotate({0.0f, 1.0f, 0.0f});
}

bool SystemWindow::loop() 
{
	static bool wheelHold = false;
	static bool leftBtnMouse = false;

	static float mouseDx;
	static float mouseDy;

	SDL_Event evt{};

	SDL_MouseButtonFlags mouseData = SDL_GetMouseState(0, 0);
	const bool* keyboardState = SDL_GetKeyboardState(0);

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

			float3 movement = evt.wheel.y * 0.5f * Camera::Forward;
			Camera::posX += movement.x;
			Camera::posY += movement.y;
			Camera::posZ += movement.z;
			break;
		}
	}// While end - Event poll loop

	/*
	{
	float3 forward{};
	float3 right{};
	float3 up{};
	float3 wordlUp{0.0f, 1.0f, 0.0f};

	forward.x = sinf(Camera::yaw) * cosf(Camera::pitch);
	forward.y = -sinf(Camera::pitch);
	forward.z = cosf(Camera::yaw) * cosf(Camera::pitch);

	Camera::Forward = forward;

	right.x = cosf(Camera::yaw);
	right.z = -sinf(Camera::yaw);

	//right = normalize(cross(wordlUp, forward));
	up		= normalize(cross(forward, right));

	Camera::right = right;
	Camera::up		= up;
	}
	*/

	/*
	float3 a = Camera::Forward;
	float3 b = { 0, 1, 0 };
	float3 c{};

	c.x = (a.z * b.y) - (a.y * b.z);
	c.y = (a.z * b.x) - (a.x * b.z);
	c.z = (a.x * b.y) - (a.y * b.x);
	Camera::right = c;

	a = Camera::Forward;
	b = Camera::right;
	c.x = (a.z * b.y) - (a.y * b.z);
	c.y = (a.z * b.x) - (a.x * b.z);
	c.z = (a.x * b.y) - (a.y * b.x);
	*/

	/*
	 * Viewport camera management.
	 * When holding the middle mouse button camera move left and right.
	 * If also holding the left shift key then camera rotations on Y 
	 * axis and X axis
	*/
	if (wheelHold) {
		if (keyboardState[SDL_SCANCODE_LSHIFT]) {
			Camera::posX += (Camera::right.x * -mouseDx * 0.01f);
			Camera::posY += (Camera::right.y * -mouseDx * 0.01f);
			Camera::posZ += (Camera::right.z * -mouseDx * 0.01f);

			Camera::posX += (Camera::up.x * mouseDy * 0.01f);
			Camera::posY += (Camera::up.y * mouseDy * 0.01f);
			Camera::posZ += (Camera::up.z * mouseDy * 0.01f);
		}
		else {
			//Camera::yaw		+= mouseDx * 0.005f;
			//Camera::pitch += mouseDy * 0.005f;
			/*
			 * Quaternion rotation
			*/
			Quaternion qYaw = Quaternion::fromAxisAngle({0.0f, 1.0f, 0.0f}, mouseDx * 0.005f);
			Camera::rotation = qYaw * Camera::rotation;

			Camera::right = Camera::rotation.rotate({1.0f, 0.0f, 0.0f});
			Quaternion qPitch = Quaternion::fromAxisAngle(Camera::right, mouseDy * 0.005f);
			Camera::rotation = qPitch * Camera::rotation;

			Camera::Forward = Camera::rotation.rotate({0.0f, 0.0f, 1.0f});
			Camera::right = Camera::rotation.rotate({1.0f, 0.0f, 0.0f});
			Camera::up = Camera::rotation.rotate({0.0f, 1.0f, 0.0f});

			Camera::rotation.toEuler(Camera::pitch, Camera::yaw, Camera::roll);
		}
	}

	if (keyboardState[SDL_SCANCODE_Q]) {
		/*
		 Quaternion  test

		float3 v = {0.0f, 0.0f, 1.0f};
		// rotation of 90deg on y axis
		Quaternion q = Quaternion::fromAxisAngle({0.0f, 1.0f, 0.0f}, mathConst::PI / 2);
		float3 rot1 = q.rotate(v);
		printf("Rotation 1: (%.7f, %.7f, %.7f)\n", rot1.x, rot1.y, rot1.z);

		// rotation of 45 deg on x axis
		q = Quaternion::fromAxisAngle({1.0f, 0.0f, 0.0f}, mathConst::PI / 4);
		float3 rot2 = q.rotate(rot1);
		printf("Rotation 2: (%.7f, %.7f, %.7f)\n", rot2.x, rot2.y, rot2.z);
		*/

		//float3 v = {0.0f, 0.0f, 1.0f};

		// Quaternion test 2

		Quaternion orientation = Quaternion::identity();
		Quaternion q1 = Quaternion::fromAxisAngle({0.0f, 1.0f, 0.0f}, mathConst::PI / 2);
		Quaternion q2 = Quaternion::fromAxisAngle({0.0f, 0.0f, 1.0f}, mathConst::PI / 4);
		Quaternion q3 = Quaternion::fromAxisAngle({0.0f, 1.0f, 0.0f}, mathConst::PI / 4);

		orientation = q3 * q2 * q1 * orientation;

		float3 v = orientation.rotate({0.0f, 0.0f, 1.0f});
		printf("Result: (%.7f, %.7f, %.7f)\n", v.x, v.y, v.z);
	}

	//SDL_Delay(1);
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