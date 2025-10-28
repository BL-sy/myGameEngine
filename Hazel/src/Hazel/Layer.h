#pragma once

#include "Event/Event.h"

namespace Hazel {
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {} // �㱻��ӵ�LayerStackʱ����
		virtual void OnDetach() {} // ���LayerStack���Ƴ�ʱ����
		virtual void OnUpdate() {} // ÿ֡���ã����ڸ��²��״̬
		virtual void OnEvent(Event& event) {}// ÿ�㴦���¼�

		inline const std::string& GetName() const { return m_DebugName; } // ��ȡ�������
	protected:
		std::string m_DebugName;
	};
}
