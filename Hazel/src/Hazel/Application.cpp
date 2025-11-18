#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include "Hazel/Renderer/Renderer.h"


namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		:m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
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

		// 3. 创建VAO并关联VBO、IBO
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

		// 3. 创建VAO并关联VBO、IBO
		m_SqrVA.reset(VertexArray::Create());
		m_SqrVA->AddVertexBuffer(squareVB);
		m_SqrVA->SetIndexBuffer(squareIB);

		// 创建shader
		// 顶点着色器src
		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main(){
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
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

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main(){
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0f);
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

	// 主循环
	void Application::Run()
	{
		while (m_Running)
		{
			// 1. 抽象清屏命令
			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.2f, 1.0f));
			RenderCommand::Clear();

			// set camera position before begin scene so matrix reflects new position
			m_Camera.SetPosition({ 0.5f, 0.5f, 0.5f });

			// 2. 渲染流程
			Renderer::BeginScene(m_Camera);

			// 提交第一个物体
			Renderer::Submit(m_BlueShader, m_SqrVA);

			// 提交第二个物体
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

			// 3. Layer更新与ImGui渲染
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}

		// 程序退出时，释放OpenGL资源
		m_VertexArray->Unbind();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}