#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include <GLFW/glfw3.h>

namespace Hazel
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        // Application��������
        m_Window = std::unique_ptr<Window>(Window::Create());
        // Application���ô����¼��Ļص�����
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    // �ص�glfw�����¼��ĺ���
    void Application::OnEvent(Event& e)
    {
        // ���¼��������������Լ�����Ҫ���ص��¼�������
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        HZ_CORE_TRACE("{0}", e.ToString());
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // ������ɫ
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }

	// �����ڹر��¼��ĺ���
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}