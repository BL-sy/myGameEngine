#include "hzpch.h"
#include "Renderer.h"


namespace Hazel {

	void Renderer::Submit(const std::shared_ptr<VertexArray>& va)
    {
        va->Bind(); // 提交时绑定VAO，分离渲染数据准备与命令执行
        RenderCommand::DrawIndexed(va);
    }
}