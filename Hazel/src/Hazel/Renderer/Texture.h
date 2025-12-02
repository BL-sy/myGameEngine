#pragma once

#include <string>

namespace Hazel {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// 纯色纹理数据
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height); // 颜色贴图
		static Ref<Texture2D> Create(const std::string& path);
	};
}