#pragma once

#include "Event/Event.h"

namespace Hazel {
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // 层被添加到LayerStack时调用
		virtual void OnDetach() {} // 层从LayerStack中移除时调用
		virtual void OnUpdate() {} // 每帧调用，用于更新层的状态
		virtual void OnEvent(Event& event) {}// 每层处理事件

		inline const std::string& GetName() const { return m_DebugName; } // 获取层的名称
	protected:
		std::string m_DebugName;
	};
}
