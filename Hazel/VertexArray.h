#pragma once

#include <memory>
#include "Buffer.h"

namespace Hazel {

	class VertexArray
	{
	public:

		// 这些都是跟VertexBuffer和IndexBuffer的接口一样的
		virtual ~VertexArray() = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;//Unbind函数一般用于debuging purposes

		// 由于一个VAO可以挖取多个VBO的数据，所以需要添加记录相关VBO引用的接口
		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffers) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		// 注意这个Create函数与VertexBuffer的Create函数一样，为static的函数，在定义的时候不需要写static关键字
		// 而且这个Create函数是在基类定义的，因为创建的窗口对象应该包含多种平台的派生类对象，所以放到了基类里
		// 而且这个基类的cpp引用了相关的派生类的头文件，相关的Create函数定义在VertexArray.cpp里完成
		static VertexArray* Create();
	};
}


