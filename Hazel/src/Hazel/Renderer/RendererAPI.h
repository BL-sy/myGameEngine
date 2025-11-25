#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace Hazel {
	class RendererAPI {
	public:
		// 渲染API类型（从Renderer移至此）
		enum class APIType { None = 0, OpenGL };

		// 纯虚接口
		virtual void Init() const = 0;
		virtual void SetClearColor(const glm::vec4& color) const = 0;
		virtual void Clear() const = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& va) const = 0;

		// 全局获取当前API类型
		inline static APIType GetAPIType() { return s_APIType; }
	private:
		static APIType s_APIType;
	};
}