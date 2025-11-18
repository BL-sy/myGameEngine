#pragma once

#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include "OrthographicCamera.h"

#include "glm/glm.hpp"

namespace Hazel {
    class Renderer
    {
    public:
        // 开始场景：缓存相机VP矩阵（全局共享）
        static void BeginScene(const OrthographicCamera& camera);
        // 结束场景：预留优化接口（批处理/排序）
        static void EndScene();
        // 提交渲染：绑定Shader/VA，上传矩阵并绘制
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

        // 查询当前渲染API类型
        inline static RendererAPI::APIType GetAPI() { return RendererAPI::GetAPIType(); }

    private:
        // 场景数据缓存：存储每帧全局渲染信息
        struct SceneData { glm::mat4 ViewProjectionMatrix; };
        static SceneData* m_SceneData;
    };
}