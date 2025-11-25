#pragma once
#include "RendererAPI.h"

namespace Hazel {

	class RenderCommand {
	public:
		static void Init() { s_RendererAPI->Init(); }
		static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
		static void Clear() { s_RendererAPI->Clear(); }
		static void DrawIndexed(const std::shared_ptr<VertexArray>& va) { s_RendererAPI->DrawIndexed(va); }
	private:
		// 初始化时创建对应平台的RendererAPI实例
		static Scope<RendererAPI> s_RendererAPI;
	};
}

