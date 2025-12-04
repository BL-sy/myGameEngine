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
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	// 静态数据实例
	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		// 初始化四边形顶点数组
		s_Data->QuadVertexArray = VertexArray::Create();

		// 四边形顶点数据
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		// 创建VBO并设置布局
		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		// 创建IBO
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		// 创建纯色纹理
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		
		// 创建纹理着色器
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

	}

	void Renderer2D::EndScene()
	{
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", tintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation, const glm::vec2& anchor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, color, rotation, anchor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, const glm::vec2& anchor)
	{
		glm::vec2 localAnchorOffset = {(anchor.x - 0.5f) * size.x, (anchor.y - 0.5f) * size.y};
		// 核心修改：直接用局部锚点（anchor）调用Rotate，不用算世界坐标！
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* RotateLocal(rotation, localAnchorOffset)  // 这里直接传 anchor（局部坐标！）
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);

		s_Data->WhiteTexture->Bind();

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, const glm::vec2& anchor, float tilingFactor, const glm::vec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, texture, rotation, anchor, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float rotation, const glm::vec2& anchor, float tilingFactor, const glm::vec4& tintColor)
	{
		

		glm::vec2 localAnchorOffset = { (anchor.x - 0.5f) * size.x, (anchor.y - 0.5f) * size.y };
		// 核心修改：直接用局部锚点（anchor）调用Rotate，不用算世界坐标！
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* RotateLocal(rotation, localAnchorOffset)  // 这里直接传 anchor（局部坐标！）
			* glm::scale(glm::mat4(1.0f), { size, 1.0f });

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", tintColor);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

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