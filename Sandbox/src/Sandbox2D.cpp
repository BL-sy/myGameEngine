#include "hzpch.h"
#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	// Square
	// 渲染初始化：VAO、VBO、IBO
	// 顶点数据与布局
	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	Hazel::BufferLayout squareVBLayout = {
		{Hazel::ShaderDataType::Float3, "a_Pos"}
	};

	// 1. 创建VBO并设置布局
	Hazel::Ref<Hazel::VertexBuffer> squareVB;
	squareVB = Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout(squareVBLayout);

	// 2. 创建IBO
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Hazel::Ref<Hazel::IndexBuffer> squareIB;
	squareIB = Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

	// 3. 创建VAO并关联VBO、IBO
	m_SquareVA = Hazel::VertexArray::Create();
	m_SquareVA->AddVertexBuffer(squareVB);
	m_SquareVA->SetIndexBuffer(squareIB);

	m_SquareShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	// 1. 抽象清屏命令
	Hazel::RenderCommand::Clear();
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	// 2. 渲染流程
	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	// 渲染一组正方形
	// 设置这一组正方形的颜色，通过imgui来设置
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	// 三角形
	Hazel::Renderer::Submit(m_SquareShader, m_SquareVA);

	Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}
