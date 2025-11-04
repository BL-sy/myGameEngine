#pragma once

#include "Hazel/Layer.h"
#include "imgui/imgui.h"  // 引入ImGui核心头文件

#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/MouseEvent.h"
#include "Hazel/Event/KeyEvent.h"

namespace Hazel {
    class HAZEL_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        // 层生命周期函数
        void OnAttach(); // 初始化ImGui上下文和设置
        void OnDetach(); // 清理ImGui资源
        void OnUpdate(); // 每帧更新ImGui
        void OnEvent(Event& e);  // 处理ImGui事件

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);

    private:
        // 记录时间（用于ImGui计算帧率和动画）
        float m_Time = 0.0f;
    };
}
