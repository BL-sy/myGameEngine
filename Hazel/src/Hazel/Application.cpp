#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// 1. 创建窗口（原有逻辑不变）
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// 2. 创建ImGuiLayer（原有逻辑不变）
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// 3. 渲染初始化：VAO、VBO、IBO（用户原有代码，补充Shader创建）
		// 3.1 顶点数据（三角形三个顶点的三维坐标）
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,  // 顶点0：左下
			 0.5f, -0.5f, 0.0f,  // 顶点1：右下
			 0.0f,  0.5f, 0.0f   // 顶点2：上中
		};

		// 3.2 索引数据（复用顶点，按顺序绘制三角形）
		unsigned int indices[3] = { 0, 1, 2 };

		// 3.3 创建VAO（管理顶点属性）
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// 3.4 创建VBO（存储顶点数据）
		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 静态数据（不频繁修改）

		// 3.5 配置顶点属性（位置属性：索引0，3个float，无归一化，步长3*float，偏移0）
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// 3.6 创建IBO（存储索引数据）
		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	// 主循环（用户原有代码）
	void Application::Run()
	{
		while (m_Running)
		{
			// 1. 清屏（原有逻辑不变）
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// 2. 渲染三角形（核心修改：绑定VAO+绘制）
			glBindVertexArray(m_VertexArray);  // 绑定VAO（自动关联VBO、IBO和顶点属性）
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);  // 绘制三角形（3个索引）

			// 3. Layer更新+ImGui渲染（原有逻辑不变）
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			if (m_ImGuiLayer)
			{
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			// 4. 交换缓冲区（原有逻辑不变）
			m_Window->OnUpdate();
		}

		// 程序退出时，释放OpenGL资源（新增）
		glDeleteVertexArrays(1, &m_VertexArray);
		glDeleteBuffers(1, &m_VertexBuffer);
		glDeleteBuffers(1, &m_IndexBuffer);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}