#include "hzpch.h"
#include "Application.h"

#include "Hazel/log.h"
#include "Hazel/Layer.h"
#include "Hazel/Input.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{
// 绑定事件函数的宏定义
// 将成员函数绑定到当前对象，并占位第一个参数
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

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

    void Application::Run() {
        while (m_Running) { // 引擎主循环：持续运行直到窗口关闭
            // 1. 清除屏幕缓冲区（设置背景色为紫色）
            glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // 2. 更新所有Layer层的逻辑（如游戏物体移动、UI刷新）
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

            // 3. 测试：全局查询鼠标位置并打印
            auto [mouseX, mouseY] = Input::GetMousePosition();
            HZ_CORE_TRACE("鼠标位置：X={0}, Y={1}", mouseX, mouseY);

            // 4. 更新GLFW窗口（处理窗口事件、交换缓冲区）
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