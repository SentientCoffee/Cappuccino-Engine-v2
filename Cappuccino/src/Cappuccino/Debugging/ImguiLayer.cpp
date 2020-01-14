#include "CappPCH.h"
#include "Cappuccino/Debugging/ImguiLayer.h"

#include "Cappuccino/Core/Application.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <glfw/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <imgui/examples/imgui_impl_glfw.h>

using namespace Capp;

ImguiLayer::ImguiLayer() : Layer("Imgui Layer") {}

void ImguiLayer::onPush() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	const auto window = Application::getInstance()->getWindow()->getGlfwWindow();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void ImguiLayer::onPop() {
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImguiLayer::begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImguiLayer::drawImgui() {}

void ImguiLayer::end() {
	ImGuiIO& io = ImGui::GetIO();
	Application* app = Application::getInstance();
	io.DisplaySize = { static_cast<float>(app->getWindow()->getWidth()), static_cast<float>(app->getWindow()->getHeight()) };

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* currentContextBackup = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(currentContextBackup);
	}
}
