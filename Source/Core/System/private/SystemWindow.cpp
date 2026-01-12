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

#include "../SystemWindow.h"
#include <ThirdParty/SDL3/SDL.h>
#include <ThirdParty/SDL3/SDL_system.h>

#include <Core/Defines/Windows/windowsAPI.h>

#include <iostream>
#include <iomanip>
#include <cstdio>

static SDL_Window* window;
static bool shouldRun = true;

SystemWindow::SystemWindow(){ }
SystemWindow::~SystemWindow() { }

void SystemWindow::initialize(const wchar* windowTitle)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
	window = SDL_CreateWindow("Optim Engine - SDL - DirectX 11", 1280, 720, SDL_WINDOW_RESIZABLE);
}

bool SystemWindow::loop() 
{
	SDL_Event evt{};

	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_EVENT_QUIT) {
			shouldRun = false;
			break;
		}
	}// While end - Event poll loop
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