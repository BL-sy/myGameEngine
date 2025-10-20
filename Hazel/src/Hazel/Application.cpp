#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        // Application创建窗口
        m_Window = std::unique_ptr<Window>(Window::Create());
        // Application设置窗口事件的回调函数
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    // 回调glfw窗口事件的函数
    void Application::OnEvent(Event& e)
    {
        // 用事件调度器，拦截自己层想要拦截的事件并处理
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        HZ_CORE_TRACE("{0}", e.ToString());
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // 背景颜色
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }

	// 处理窗口关闭事件的函数
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}