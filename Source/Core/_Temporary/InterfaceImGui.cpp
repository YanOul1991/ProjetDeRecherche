/* ======================================================================================
 *  InterfaceImGui.cpp:
 *
 *  By:
 *    Yanis Oulmane
====================================================================================== */

#include "Core/Object/Camera/Camera.h"

#include "ThirdParty/imgui/imgui_impl_sdl3.h"
#include "ThirdParty/imgui/imgui_impl_dx11.h"
#include "ThirdParty/imgui/imgui.h"

//#include "ThirdParty/SDL3/SDL.h"
#include "Core/System/Application.h"
#include "Core/_Temporary/InterfaceImGui.h"

InterfaceImGui::InterfaceImGui()
{}

InterfaceImGui::~InterfaceImGui()
{}

void InterfaceImGui::initWindow(void* sdlWindow)
{
	IMGUI_CHECKVERSION(); 
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForD3D(reinterpret_cast<SDL_Window*>(sdlWindow));

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void InterfaceImGui::initDirectX(void* pDevice, void* pContext)
{
	ImGui_ImplDX11_Init(
		reinterpret_cast<ID3D11Device*>(pDevice), 
		reinterpret_cast<ID3D11DeviceContext*>(pContext)
	);
}

void InterfaceImGui::update()
{
	ImGui_ImplDX11_NewFrame(); 
	ImGui_ImplSDL3_NewFrame(); 
	ImGui::NewFrame();

	ImGui::Begin("Debug"); 
	ImGui::SetWindowFontScale(1.05f);

	if (ImGui::CollapsingHeader("System Stats")) {
		ImGui::Text("\tRuntime    : %.2f", Application::getRuntime());
		ImGui::Text("\tDelta Time : %.8f", Application::getDeltaTime());
		ImGui::Text("\tFrameRate  : %d", static_cast<int>(1.0f / Application::getDeltaTime()));
	}

	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::Text("Position : %.7f, %7f, %7f", Camera::position.x, Camera::position.y, Camera::position.z);
		ImGui::Text("Forward  : %.7f, %7f, %7f", Camera::forward.x, Camera::forward.y, Camera::forward.z);
		ImGui::Text("Right    : %.7f, %7f, %7f", Camera::right.x, Camera::right.y, Camera::right.z);
		ImGui::Text("Up       : %.7f, %7f, %7f", Camera::up.x, Camera::up.y, Camera::up.z);
	}

	float pitch;
	float yaw;
	float roll;

	Camera::rotation.toEuler(pitch, yaw, roll);

	ImGui::Text("Euler angle (%.7f, %.7f, %.7f)", pitch * 180/mathConst::PI, yaw * 180/mathConst::PI, roll * 180/mathConst::PI);

	ImGui::End();
	ImGui::Render(); 

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}