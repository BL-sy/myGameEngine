#include "hzpch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		// 生成正交投影矩阵：轴对齐，无透视效果
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewMatrix = glm::mat4(1.0f); // 初始视图矩阵为单位矩阵（原点无旋转）
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // 预计算VP矩阵
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); // 更新正交投影矩阵
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // 重新计算VP矩阵
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		// 1. 构建相机变换矩阵：先旋转（绕Z轴）后平移（符合2D操作直觉）
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		// 2. 视图矩阵 = 相机变换矩阵的逆矩阵
		// 原理：相机移动本质是"世界反向移动"（相机左移=世界右移），逆矩阵实现该逻辑
		m_ViewMatrix = glm::inverse(transform);

		// 3. 预计算VP矩阵：避免渲染时重复计算，提升性能
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}