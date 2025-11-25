#pragma once

#include "Hazel/Renderer/RendererAPI.h"
#include <glad/glad.h>

namespace Hazel {

	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void SetClearColor(const glm::vec4& color) const override 
		{
			glClearColor(color.r, color.g, color.b, color.a);
		}

		virtual void Clear() const override 
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}

		virtual void DrawIndexed(const Ref<VertexArray>& va) const override 
		{
			glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	};
}