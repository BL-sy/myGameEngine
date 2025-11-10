#pragma once
#include <string>

namespace Hazel {

	class Shader
	{
	public:
		Shader(std::string& vertexSrc, std::string& fragmentSrc);
		~Shader();

		void Bind();
		void UnBind();

	private:
		// Shader Program ID
		uint32_t m_RendererID = 0;
	};
}

