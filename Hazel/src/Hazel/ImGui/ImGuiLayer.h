#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"


namespace Hazel {

	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();// 启动ImGui帧（在所有Layer的OnImGuiRender前调用）
		void End();// 结束ImGui帧（在所有Layer的OnImGuiRender后调用）
	private:
		float m_Time = 0.0f;
	};

}