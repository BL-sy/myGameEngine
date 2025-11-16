#pragma once

namespace Hazel {

	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	// 计算ShaderDataType对应的数据字节大小（跨平台通用，不依赖具体API）
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;       // 属性名称（需与Shader中输入变量名一致，如"a_Position"）
		ShaderDataType Type;    // 属性数据类型（如Float3）
		uint32_t Size;          // 属性占用字节大小（由ShaderDataTypeSize计算）
		uint32_t Offset;        // 属性在顶点数据中的偏移量（字节，自动计算）
		bool Normalized;        // 是否归一化（仅整型数据有效，将0~255映射为0.0~1.0）

		BufferElement() {};
		BufferElement(ShaderDataType type, const std::string name, bool normalized = false)
			:Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		// 获取属性的组件数量（如Float3返回3，Mat4返回16）
		// 作用：告诉渲染API该属性由多少个基础组件组成，用于glVertexAttribPointer的第二个参数
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout(){}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			:m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		// 获取单个顶点的总步长（字节）：即一个顶点所有属性的总大小，用于glVertexAttribPointer的第五个参数
		inline uint32_t GetStride() const { return m_Stride; }
		// 获取所有顶点属性元素列表（供平台实现遍历配置）
		inline const std::vector<BufferElement>& GetElement() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		// 自动计算每个属性的偏移量和顶点总步长
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (BufferElement& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	// 顶点缓冲区
	class VertexBuffer
	{
	public:
		VertexBuffer() {};
		virtual ~VertexBuffer() {};

		virtual void Bind() const  = 0;
		virtual void UnBind() const = 0;

		// 获取顶点布局（上层可查询布局信息，平台实现可读取布局配置属性）
		virtual const BufferLayout& GetBufferLayout() const = 0;
		// 设置顶点布局（将布局与缓冲绑定，必须在绑定缓冲后调用）
		virtual void SetLayout(BufferLayout& layout) = 0;

		// 工厂函数：根据当前渲染API创建平台对应的VertexBuffer实例
		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	// 索引缓冲区
	class IndexBuffer
	{
	public:
		IndexBuffer() {};
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		// 获取索引数量（供绘制时使用，如glDrawElements的第二个参数）
		virtual uint32_t GetCount() const = 0;

		// 工厂函数：根据当前渲染API创建平台对应的IndexBuffer实例
		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};
}