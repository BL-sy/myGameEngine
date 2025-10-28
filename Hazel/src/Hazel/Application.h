#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"

namespace Hazel {
    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

		// 回调glfw窗口事件的函数
        void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;

		LayerStack m_LayerStack;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}

