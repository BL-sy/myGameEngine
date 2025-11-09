#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Hazel/Application.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// 初始化ImGui上下文
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		// 开启键盘控制
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// 开启Docking特性
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// 开启Viewports特性（多独立窗口）
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		//  配置样式：让跨平台窗口（如Windows原生窗口）与ImGui风格统一
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;          // 取消窗口圆角，匹配原生窗口
			style.Colors[ImGuiCol_WindowBg].w = 1.0f; // 确保窗口背景不透明
		}

		// 对接Hazel的GLFW窗口
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		if (window == nullptr) {
			HZ_CORE_ERROR("GLFW window is null - cannot initialize ImGui!");
		}

		// 初始化ImGui-GLFW后端（处理窗口大小、输入事件等）
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		// 初始化ImGui-OpenGL3后端（指定GLSL版本，需与Hazel渲染管线匹配）
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();  // OpenGL后端新帧
		ImGui_ImplGlfw_NewFrame();     // GLFW后端新帧
		ImGui::NewFrame();             // ImGui核心新帧
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		// 设置ImGui渲染窗口大小（与Hazel主窗口一致）
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		// 1. 渲染ImGui绘制数据
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 2. 处理跨平台Viewports（若开启）
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_ctx = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();  // 更新原生窗口状态
			ImGui::RenderPlatformWindowsDefault(); // 渲染原生窗口
			glfwMakeContextCurrent(backup_ctx);   // 恢复Hazel的GL上下文
		}
	}
	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);// 显示ImGui官方Demo窗口
	}

}