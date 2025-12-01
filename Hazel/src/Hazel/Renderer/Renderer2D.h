#pragma once

#include "OrthographicCamera.h"

namespace Hazel {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation); // 锚点默认(0.5,0.5)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation); // 接受2DPos指定锚点
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation); // 指定锚点
	private:
		static glm::mat4 RotateLocal(float rotation, const glm::vec2& localAnchorOffset); // 旋转：本地锚点
	};
}