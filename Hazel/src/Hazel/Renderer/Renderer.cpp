#include "hzpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		// 缓存相机预计算的VP矩阵，避免每帧多次获取
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() 
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		// 上传VP矩阵到Shader的uniform变量（所有物体共享）
		// 动态转换到OpenGLShader，调用平台相关的Uniform上传方法
		auto openglShader = std::dynamic_pointer_cast<OpenGLShader>(shader);
		openglShader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		openglShader->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray); // 执行绘制命令
	}
}