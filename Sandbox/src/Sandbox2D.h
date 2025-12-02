#pragma once
#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Hazel::Timestep ts) override;

	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::Ref<Hazel::Shader> m_SquareShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = { 0.6f, 0.3f, 0.8f, 1.0f };

	float m_SquareMoveSpeed = 1.0f;
	glm::vec2 m_SquarePosition = { 0.0f, 0.0f };
	float m_SquareRotateSpeed = 90.0f;
	float m_Rotation = 0.0f;
};