#pragma once

#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/OrthographicCamera.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class OrthographicCameraController 
	{
	public:
		// 创建一个正交相机控制器
		// aspectRatio: 宽高比
		// rotation: 是否允许旋转, 默认不允许
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCameara() { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio; // 宽高比
		float m_ZoomLevel = 1.0f; // 缩放等级, 默认1.0f, 越大视野越大
		OrthographicCamera m_Camera;

		bool m_Rotation; // 是否允许旋转

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}