#include "hzpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath); // 1. 读文件内容到字符串
		auto shaderSources = PreProcess(source); // 2. 拆分顶点/片段源码
		Compile(shaderSources); // 3. 编译+链接

		// 从文件路径中提取Shader名称	
		// /assets/shaders/Texture.glsl -> Texture
		auto lastSlash = filepath.find_last_of("/\\"); // 找最后一个斜杠
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastPoint = filepath.rfind('.'); // 找最后一个点

		auto count = lastPoint == std::string::npos ? filepath.size() - lastSlash : lastPoint - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);// 二进制模式读
		if (in)
		{
			in.seekg(0, std::ios::end); // 光标移到文件末尾
			result.resize(in.tellg()); // 字符串大小设为文件长度（tellg()获取当前光标位置=文件大小）
			in.seekg(0, std::ios::beg); // 光标移回文件开头
			in.read(&result[0], result.size()); // 读取文件内容到字符串
			in.close(); // 关闭文件
		}
		else
		{
			HZ_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources; // 存储“Shader类型→源码”的映射
		const char* typeToken = "#type"; // 标记Shader类型的关键词
		size_t typeTokenLength = strlen(typeToken); // 关键词长度（5个字符）
		size_t pos = source.find(typeToken, 0); // 从开头找第一个“#type”

		while (pos != std::string::npos) // 只要能找到“#type”，就继续循环
		{
			size_t eol = source.find_first_of("\r\n", pos); // 找“#type”后面的换行符（结束位置）
			HZ_CORE_ASSERT((eol != std::string::npos), "Syntax error"); // 没找到换行符=语法错误
			size_t begin = pos + typeTokenLength + 1; // 跳过“#type”和空格，得到类型字符串的起始位置
			std::string type = source.substr(begin, eol - begin); // 截取“#type”后面的类型字符串（如“vertex”）
			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified"); // 类型无效则断言

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); // 找换行符后第一个非空字符（源码起始位置）
			pos = source.find(typeToken, nextLinePos); // 找下一个“#type”（准备拆分下一个Shader）

			// 截取当前Shader的源码：从nextLinePos到下一个“#type”之前（或文件末尾）
			shaderSources[ShaderTypeFromString(type)] = source.substr(
				nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)
			);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram(); // 创建OpenGL程序对象（最终的Shader程序）
		std::array<GLenum, 2> glShaderIDs; // 存储每个Shader的ID（用于后续删除）
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "Only supports 2 shaders for now");

		int shaderIndex = 0;
		// 遍历哈希表，编译每个Shader（顶点+片段）
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first; // Shader类型（GL_VERTEX_SHADER/GL_FRAGMENT_SHADER）
			const std::string& source = kv.second; // 对应的Shader源码

			GLuint shader = glCreateShader(type); // 创建对应类型的Shader对象
			const GLchar* sourceCStr = source.c_str(); // 字符串转C风格字符串（OpenGL需要）
			glShaderSource(shader, 1, &sourceCStr, 0); // 把源码传入Shader对象
			glCompileShader(shader); // 编译Shader

			// 编译错误检查
			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader); // 编译失败，删除这个Shader
				HZ_CORE_ERROR("{0}", infoLog.data());
				HZ_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader); // 把编译好的Shader附加到程序对象上
			glShaderIDs[shaderIndex++] = shader; // 记录Shader ID，后续用于删除
		}

		m_RendererID = program; // 保存程序对象ID（后续Bind/UnBind用）
		glLinkProgram(program); // 链接程序对象（把顶点和片段Shader合并成一个可用的程序）

		// 链接错误检查
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			// 链接失败，删除程序和所有Shader
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);
			for (auto id : glShaderIDs) 
				glDeleteShader(id);
			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// 链接成功后，分离Shader（程序对象已包含所需信息，Shader对象可删除）
		for (auto id : glShaderIDs) 
			glDetachShader(program, id);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}