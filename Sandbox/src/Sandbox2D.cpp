#include "hzpch.h"
#include "Sandbox2D.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = Hazel::Texture2D::Create("assets/textures/asuka.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		m_SquarePosition.x -= m_SquareMoveSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		m_SquarePosition.x += m_SquareMoveSpeed * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		m_SquarePosition.y += m_SquareMoveSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		m_SquarePosition.y -= m_SquareMoveSpeed * ts;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_Z))
		m_Rotation -= m_SquareRotateSpeed * ts;
	else if (Hazel::Input::IsKeyPressed(HZ_KEY_C))
		m_Rotation += m_SquareRotateSpeed * ts;

	// Render
	// 1. 抽象清屏命令
	Hazel::RenderCommand::Clear();
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

	// 2. 渲染流程
	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	
	// 3. 绘制四边形
	Hazel::Renderer2D::DrawQuad(m_SquarePosition + glm::vec2{ 0.5f, 0.5f }, { 0.5f, 0.5f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 0.8f }, { 0.0f, 1.0f }, m_Rotation, m_Texture);
	Hazel::Renderer2D::DrawQuad(m_SquarePosition, { 1.0f, 1.0f }, m_SquareColor, { 0.0f, 0.0f }, m_Rotation);

	// 4. 结束场景
	Hazel::Renderer2D::EndScene();
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
