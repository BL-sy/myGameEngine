#pragma once

#include "Hazel/Renderer/VertexArray.h"

#include <memory>
#include <vector>

namespace Hazel {
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; };
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };

    private:
        uint32_t m_RendererID = 0; // OpenGL VAO对象ID

        std::vector<Ref<VertexBuffer>> m_VertexBuffers; // 关联的VBO列表
        Ref<IndexBuffer> m_IndexBuffer; // 关联的IBO
        uint32_t m_VertexAttribIndex = 0; // 顶点属性索引（自动递增，避免手动管理）
    };
}