#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Hazel {

	struct Quad
	{
		glm::vec2 Position;
		glm::vec2 Size;
		glm::vec4 Color;
		glm::vec2 Anchor; // 锚点(0~1)
		float Rotation;   // 旋转角度（弧度制）
		Ref<Texture2D> Texture; // 纹理贴图
	};

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// 绘制四边形的多种重载方式
		// 参数绘制四边形
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation); // 锚点默认(0.5,0.5)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation); // 接受2DPos指定锚点
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation); // 指定锚点

		// 带纹理
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, Ref<Texture2D> texture); // 锚点默认(0.5,0.5)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation, Ref<Texture2D> texture); // 接受2DPos指定锚点
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation, Ref<Texture2D> texture); // 指定锚点

	private:
		static glm::mat4 RotateLocal(float rotation, const glm::vec2& localAnchorOffset); // 旋转：本地锚点
	};
}