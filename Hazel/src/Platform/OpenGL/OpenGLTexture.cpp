#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>


namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1); // 翻转Y轴（正确，Shader纹理坐标Y轴与图片相反）
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HZ_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		// 动态设置纹理格式
		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
		if (channels == 3)
		{
			internalFormat = GL_RGB8; // 内部存储格式：RGB8（每个通道8位）
			dataFormat = GL_RGB;      // 数据格式：RGB
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8; // 支持透明通道
			dataFormat = GL_RGBA;
		}
		HZ_CORE_ASSERT(internalFormat && dataFormat, "Unsupported image format!");

		// 创建纹理并分配存储（使用动态格式）
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		// 纹理过滤参数
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 上传纹理数据（使用动态格式）
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}

}