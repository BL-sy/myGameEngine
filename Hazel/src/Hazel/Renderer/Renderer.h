#pragma once

#include "VertexArray.h"
#include "RenderCommand.h"

namespace Hazel {

    class Renderer 
    {
    public:
        static void BeginScene() {} // 预留接口，后续扩展环境设置
        static void EndScene() {}   // 预留接口，后续扩展优化
        static void Submit(const std::shared_ptr<VertexArray>& va);

        // 复用RendererAPI的API类型查询
        inline static RendererAPI::APIType GetAPI() {
            return RendererAPI::GetAPIType();
        }
    };
}