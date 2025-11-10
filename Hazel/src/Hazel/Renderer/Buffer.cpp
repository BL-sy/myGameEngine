#include "hzpch.h"

#include "Buffer.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	VertexBuffer* Hazel::VertexBuffer::Create(float* vertices, uint32_t size)
	{
		// 决定使用哪个API，实例化的类型
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		HZ_CORE_ASSERT(false, "None is now not suported!");
			case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(vertices, size);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}

	IndexBuffer* Hazel::IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		// 决定使用哪个API，实例化的类型
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		HZ_CORE_ASSERT(false, "None is now not suported!");
		case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(indices, size);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!")
			return nullptr;
	}
}