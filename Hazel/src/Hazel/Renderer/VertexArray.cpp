#include "hzpch.h"
#include "VertexArray.h"
#include "Hazel/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel {
	VertexArray* VertexArray::Create()
	{
		// 决定使用哪个API，实例化的类型
		switch (Renderer::GetAPI())
		{
			case RendererAPI::APIType::None:		HZ_CORE_ASSERT(false, "None is now not suported!");
			case RendererAPI::APIType::OpenGL:		return new OpenGLVertexArray(); // 这里size是count
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}
}
