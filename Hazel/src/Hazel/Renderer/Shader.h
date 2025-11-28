#pragma once
#include <unordered_map>
#include <string>
#include "Hazel/Core.h"

namespace Hazel {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name); // 根据名称获取Shader

		bool Exists(const std::string& name) const; // 检查Shader是否存在
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders; // 存储“Shader名称→Shader对象”的映射
	};
}

