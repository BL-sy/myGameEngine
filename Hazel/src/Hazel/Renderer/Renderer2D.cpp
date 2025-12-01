#include "hzpch.h"

#include "Renderer2D.h"

#include "RenderCommand.h"

#include "VertexArray.h"
#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	// Renderer2D内部数据
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	// 静态数据实例
	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		// 初始化四边形顶点数组
		s_Data->QuadVertexArray = VertexArray::Create();

		// 四边形顶点数据
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		// 创建VBO并设置布局
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position"}
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		// 创建IBO
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// 创建平面颜色着色器
		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
	}

	
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, { 0.5f, 0.5f }, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, anchor, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const glm::vec2& anchor, float rotation)
	{
		glm::vec2 localAnchorOffset = {(anchor.x - 0.5f) * size.x, (anchor.y - 0.5f) * size.y};
		// 核心修改：直接用局部锚点（anchor）调用Rotate，不用算世界坐标！
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* RotateLocal(rotation, localAnchorOffset)  // 这里直接传 anchor（局部坐标！）
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_Data->FlatColorShader->SetMat4("u_Transform", transform);
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	glm::mat4 Renderer2D::RotateLocal(float rotation, const glm::vec2& localAnchorOffset) {
		// 本地空间旋转逻辑：平移到本地锚点 → 旋转 → 平移回原点
		return glm::translate(glm::mat4(1.0f), glm::vec3(localAnchorOffset, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, -1.0f))
			* glm::translate(glm::mat4(1.0f), glm::vec3(-localAnchorOffset, 0.0f));
	}

}