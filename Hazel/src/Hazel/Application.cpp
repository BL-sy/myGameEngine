#include "hzpch.h"
#include "Application.h"

#include "Hazel/log.h"
#include "Hazel/Layer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{
// ���¼������ĺ궨��
// ����Ա�����󶨵���ǰ���󣬲�ռλ��һ������
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

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    // �ص�glfw�����¼��ĺ���
    void Application::OnEvent(Event& e)
    {
        // ���ش��ڹر��¼�
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
            // ������ɫ
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            // ����ÿһ��
            for(Layer* layer : m_LayerStack)
                layer->OnUpdate();
            
            // ���´���
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