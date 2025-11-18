#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene(const OrthographicCamera& camera)
    {
        // 缓存相机预计算的VP矩阵，避免每帧多次获取
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() 
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->Bind();
        // 上传VP矩阵到Shader的uniform变量（所有物体共享）
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray); // 执行绘制命令
    }
}