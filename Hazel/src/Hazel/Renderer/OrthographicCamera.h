#pragma once
#include <glm/glm.hpp>

namespace Hazel {
	// 2D正交摄像机：无透视畸变，仅绕Z轴旋转
	class OrthographicCamera
	{
	public:
		// 构造函数：初始化正交投影范围（left/right/bottom/top），Z轴范围[-1,1]
		OrthographicCamera(float left, float right, float bottom, float top);

		// 设置正交投影范围，重计算投影矩阵和VP矩阵
		void SetProjection(float left, float right, float bottom, float top);

		// 位置/旋转读写：修改后自动重计算矩阵
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		// 矩阵读写：获取预计算的投影/视图/VP矩阵
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		// 核心：重计算视图矩阵和VP矩阵
		void RecalculateViewMatrix();

		glm::mat4 m_ProjectionMatrix;    // 正交投影矩阵
		glm::mat4 m_ViewMatrix;          // 视图矩阵
		glm::mat4 m_ViewProjectionMatrix;// 预计算VP矩阵（Projection * View）
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f }; // 相机位置（Z轴用于层叠）
	private:
		float m_Rotation = 0.0f;         // 仅绕Z轴旋转（2D场景）
	};
}