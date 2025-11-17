#include "hzpch.h"

#include "OpenGLVertexArray.h"
#include "Hazel/Renderer/Buffer.h"

#include <glad/glad.h>

namespace Hazel {

	// 平台工具函数：ShaderDataType转OpenGL原生类型（仅在OpenGL实现中使用）
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:     return GL_FLOAT;
		case ShaderDataType::Float2:    return GL_FLOAT;
		case ShaderDataType::Float3:    return GL_FLOAT;
		case ShaderDataType::Float4:    return GL_FLOAT;
		case ShaderDataType::Mat3:      return GL_FLOAT;
		case ShaderDataType::Mat4:      return GL_FLOAT;
		case ShaderDataType::Int:       return GL_INT;
		case ShaderDataType::Int2:      return GL_INT;
		case ShaderDataType::Int3:      return GL_INT;
		case ShaderDataType::Int4:      return GL_INT;
		case ShaderDataType::Bool:      return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		// 创建OpenGL VAO对象
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		// 释放VAO资源
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer, "VertexBuffer is null!");
		HZ_CORE_ASSERT(vertexBuffer->GetBufferLayout().GetElement().size() > 0, "VertexBuffer layout is empty!");

		// 绑定VAO和VBO（VAO会记录VBO的绑定状态）
		Bind();
		vertexBuffer->Bind();

		// 自动配置顶点属性（从VBO的BufferLayout中读取配置）
		const auto& layout = vertexBuffer->GetBufferLayout();
		for (const auto& element : layout.GetElement())
		{
			glEnableVertexAttribArray(m_VertexAttribIndex);
			glVertexAttribPointer(
				m_VertexAttribIndex,
				element.GetComponentCount(), // 组件数量（如vec3为3）
				ShaderDataTypeToOpenGLBaseType(element.Type), // OpenGL数据类型
				element.Normalized ? GL_TRUE : GL_FALSE, // 是否归一化
				layout.GetStride(), // 顶点步长（单个顶点总字节数）
				(const void*)element.Offset // 属性偏移量
			);
			m_VertexAttribIndex++;
		}

		// 存储VBO引用，防止智能指针提前释放
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		HZ_CORE_ASSERT(indexBuffer, "IndexBuffer is null!");

		// 绑定VAO和IBO（VAO会记录IBO的绑定状态）
		Bind();
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}