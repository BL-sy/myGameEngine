#include "hzpch.h"
#include "Application.h"

#include "Hazel/log.h"
#include "Hazel/Layer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{
// 绑定事件函数的宏定义
// 将成员函数绑定到当前对象，并占位第一个参数
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

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    // 回调glfw窗口事件的函数
    void Application::OnEvent(Event& e)
    {
        // 拦截窗口关闭事件
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        HZ_CORE_TRACE("{0}", e.ToString());

        for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // 背景颜色
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            // 更新每一层
            for(Layer* layer : m_LayerStack)
                layer->OnUpdate();
            
            // 更新窗口
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