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

	/*
	// Docking space
	ImGuiWindowFlags	windowFlags = ImGuiWindowFlags_NoDocking;
	ImGuiViewport*		viewPort		= ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewPort->WorkPos);
	ImGui::SetNextWindowSize(viewPort->WorkSize);
	ImGui::SetNextWindowViewport(viewPort->ID);

	windowFlags |=	ImGuiWindowFlags_NoTitleBar | 
									ImGuiWindowFlags_NoCollapse | 
									ImGuiWindowFlags_NoResize | 
									ImGuiWindowFlags_NoMove | 
									ImGuiWindowFlags_NoBringToFrontOnFocus | 
									ImGuiWindowFlags_NoNavFocus |
									ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); 
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::Begin("DockSpaceHost", nullptr, windowFlags); 
	ImGui::PopStyleVar(2);

	ImGuiID dockSpaceId = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0, 0), ImGuiDockNodeFlags_None);

	ImGui::End();
	
	*/

	ImGui::Begin("Debug"); 
	ImGui::SetWindowFontScale(1.05f);

	if (ImGui::CollapsingHeader("System Stats")) {
		ImGui::Text("\tRuntime    : %.2f", Application::getRuntime());
		ImGui::Text("\tDelta Time : %.8f", Application::getDeltaTime());
		ImGui::Text("\tFrameRate  : %d", static_cast<int>(1.0f / Application::getDeltaTime()));
	}

	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::SliderFloat("PositionX", &Camera::posX, -10.0f, 10.0f);
		ImGui::SliderFloat("PositionY", &Camera::posY, -10.0f, 10.0f);
		ImGui::SliderFloat("PositionZ", &Camera::posZ, -10.0f, 10.0f);
		ImGui::Spacing();
		ImGui::SliderFloat("RotationX", &Camera::rotX, -10.0f, 10.0f);
		ImGui::SliderFloat("RotationY", &Camera::rotY, -10.0f, 10.0f);
		ImGui::SliderFloat("RotationZ", &Camera::rotZ, -10.0f, 10.0f);
		ImGui::Spacing();
		ImGui::Text("Camera Forward: %.7f, %7f, %7f", Camera::Forward.x, Camera::Forward.y, Camera::Forward.z);
		ImGui::Spacing();
		ImGui::Text("Camera Right: %.7f, %7f, %7f", Camera::right.x, Camera::right.y, Camera::right.z);
		ImGui::Spacing();
		ImGui::Text("Camera Up: %.7f, %7f, %7f", Camera::up.x, Camera::up.y, Camera::up.z);
	}

	ImGui::End();

	ImGui::Render(); 
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}