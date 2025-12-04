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
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, const glm::vec2& anchor = {0.5f, 0.5f});
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, const glm::vec2& anchor = {0.5f, 0.5f});
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, const glm::vec2& anchor = { 0.5f, 0.5f }, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, const glm::vec2& anchor = { 0.5f, 0.5f }, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

	private:
		static glm::mat4 RotateLocal(float rotation, const glm::vec2& localAnchorOffset); // 旋转：本地锚点
	};
}