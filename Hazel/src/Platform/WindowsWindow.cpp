#include "hzpch.h"
#include "WindowsWindow.h"

#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include <Glad/glad.h>

namespace Hazel {

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description) 
    {
        HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    // ��WindowsWindow���ඨ����Window���������ĺ���
    Window* Window::Create(const WindowProps& props) 
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) 
    {
        Init(props);
    }

    // Windows�������ٴ���
    WindowsWindow::~WindowsWindow() 
    {
        Shutdown();
    }

    // Windows���ڳ�ʼ������
    void WindowsWindow::Init(const WindowProps& props) 
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        HZ_CORE_INFO("Create window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
            // ����glfw����ص�����
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        // ��������//////////////////////////////////////////////
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        // ����glfw��ǰ��������
        glfwMakeContextCurrent(m_Window);
        //����Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        // ��֤���ؽ������ӡOpenGL�汾���Կ���Ϣ��
        HZ_CORE_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        HZ_CORE_INFO("OpenGL Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

        // ���ô����û�ָ��Ϊ�������ݽṹ��ĵ�ַ
        // ���������ڻص������з��ʴ�������
        glfwSetWindowUserPointer(m_Window, &m_Data);// ���ô��ڻص���������
        SetVSync(true);// Ĭ�����ô�ֱͬ��

        SetCallbacks();
    }

	// ����Windows���ڻص�����
    void WindowsWindow::SetCallbacks()
    {
        // ���ڴ�С�ı��¼��ص�
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                // glfwGetWindowUserPointer��ȡvoid*ָ�����ת��Ϊ��glfwSetWindowUserPointer�Զ����������ͣ�
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                // ����WindowResizeEvent�¼�����
                WindowResizeEvent event(width, height);
                // �ص�Application��OnEvent�����������¼���Ϊ��OnEvent�Ĳ���
                data.EventCallback(event);
            });

        // ���ڹر��¼��ص�
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        // ���̰����¼��ص�
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        // ��갴ť�¼��ص�
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                }
            });

        // �������¼��ص�
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        // ����ƶ��¼��ص�
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
    }

    // Windows���ڸ��´���
    void WindowsWindow::OnUpdate() 
    {
        glfwPollEvents();// �������¼�,��ѯ�¼�
        glfwSwapBuffers(m_Window);// ����ǰ�󻺳���
    }

    // ���ô�ֱͬ��
    void WindowsWindow::SetVSync(bool enabled) {
        if(enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }
    bool WindowsWindow::IsVSync() const {
        // ���ش�ֱͬ��״̬�Ĵ���
        return m_Data.VSync;
    }

    void WindowsWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
    }
}	
