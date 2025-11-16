#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <glad/glad.h>


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
		// 顶点数据与布局
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Pos"},
			{ShaderDataType::Float4, "a_Color"}
		};

		// 1. 创建VBO并设置布局
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(layout);

		// 2. 创建IBO
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// 3. 创建VAO并关联VBO、IBO（核心修改：纯抽象接口）
		m_VertexArray.reset(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// blue
		// 渲染初始化：VAO、VBO、IBO
		// 顶点数据与布局
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f, 
			 0.75f, -0.75f, 0.0f, 
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		BufferLayout squareVBLayout = {
			{ShaderDataType::Float3, "a_Pos"}
		};

		// 1. 创建VBO并设置布局
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(squareVBLayout);

		// 2. 创建IBO
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		// 3. 创建VAO并关联VBO、IBO（核心修改：纯抽象接口）
		m_SqrVA.reset(VertexArray::Create());
		m_SqrVA->AddVertexBuffer(squareVB);
		m_SqrVA->SetIndexBuffer(squareIB);

		// 创建shader
		// 顶点着色器src
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main(){
				gl_Position = vec4(a_Position, 1.0f);
				v_Position = a_Position;
				v_Color = a_Color;
			}
		)";
		// 片段着色器
		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main(){
				color = vec4(v_Position * 0.5 + 0.5, 1.0f);
				color = v_Color;
			}
		)";

		// Instantiate the main shader to avoid null dereference later in Run()
		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		// 创建shader2
		// 顶点着色器src
		std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			void main(){
				gl_Position = vec4(a_Position, 1.0f);
				v_Position = a_Position;
			}
		)";
		// 片段着色器
		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main(){
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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

			m_BlueShader->Bind();
			m_SqrVA->Bind();
			glDrawElements(GL_TRIANGLES, m_SqrVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			// 每帧绑定着色器
			m_Shader->Bind();
			// 渲染三角形（核心修改：绑定VAO+绘制）
			m_VertexArray->Bind(); // 绑定VAO（自动关联VBO、IBO和顶点属性）
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);  // 绘制三角形（3个索引）

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
		m_VertexArray->Unbind();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}