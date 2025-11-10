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

		// 创建窗口
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		// 创建ImGuiLayer
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// 渲染初始化：VAO、VBO、IBO
		// 顶点数据（三角形三个顶点的三维坐标）
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,  // 顶点0：左下
			 0.5f, -0.5f, 0.0f,  // 顶点1：右下
			 0.0f,  0.5f, 0.0f   // 顶点2：上中
		};

		// 索引数据（复用顶点，按顺序绘制三角形）
		uint32_t indices[3] = { 0, 1, 2 };

		// 创建VAO（管理顶点属性）
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		// 创建VBO（存储顶点数据）
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		// 配置顶点属性（位置属性：索引0，3个float，无归一化，步长3*float，偏移0）
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		// 创建IBO（存储索引数据）
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		// 创建shader
		// 顶点着色器src
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main(){
				gl_Position = vec4(a_Position, 1.0f);
				v_Position = a_Position;
			}
		)";
		// 片段着色器
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main(){
				color = vec4(v_Position * 0.5 + 0.5, 1.0f);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			// 清屏（原有逻辑不变）
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// 每帧绑定着色器
			m_Shader->Bind();
			// 渲染三角形（核心修改：绑定VAO+绘制）
			glBindVertexArray(m_VertexArray);  // 绑定VAO（自动关联VBO、IBO和顶点属性）
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);  // 绘制三角形（3个索引）

			// Layer更新+ImGui渲染（原有逻辑不变）
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			if (m_ImGuiLayer)
			{
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			// 交换缓冲区（原有逻辑不变）
			m_Window->OnUpdate();
		}

		// 程序退出时，释放OpenGL资源（新增）
		glDeleteVertexArrays(1, &m_VertexArray);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}