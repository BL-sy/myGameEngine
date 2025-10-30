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

    // 在WindowsWindow子类定义在Window父类声明的函数
    Window* Window::Create(const WindowProps& props) 
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props) 
    {
        Init(props);
    }

    // Windows窗口销毁代码
    WindowsWindow::~WindowsWindow() 
    {
        Shutdown();
    }

    // Windows窗口初始化代码
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
            // 设置glfw错误回调函数
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        // 创建窗口//////////////////////////////////////////////
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        // 设置glfw当前的上下文
        glfwMakeContextCurrent(m_Window);
        //加载Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        // 验证加载结果（打印OpenGL版本和显卡信息）
        HZ_CORE_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        HZ_CORE_INFO("OpenGL Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

        // 设置窗口用户指针为窗口数据结构体的地址
        // 这样可以在回调函数中访问窗口数据
        glfwSetWindowUserPointer(m_Window, &m_Data);// 设置窗口回调函数代码
        SetVSync(true);// 默认启用垂直同步

        SetCallbacks();
    }

	// 设置Windows窗口回调函数
    void WindowsWindow::SetCallbacks()
    {
        // 窗口大小改变事件回调
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                // glfwGetWindowUserPointer获取void*指针可以转换为由glfwSetWindowUserPointer自定义数据类型，
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                // 创建WindowResizeEvent事件对象
                WindowResizeEvent event(width, height);
                // 回调Application的OnEvent函数，并将事件作为其OnEvent的参数
                data.EventCallback(event);
            });

        // 窗口关闭事件回调
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);
            });

        // 键盘按键事件回调
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

        // 鼠标按钮事件回调
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

        // 鼠标滚轮事件回调
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });

        // 鼠标移动事件回调
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseMovedEvent event((float)xPos, (float)yPos);
                data.EventCallback(event);
            });
    }

    // Windows窗口更新代码
    void WindowsWindow::OnUpdate() 
    {
        glfwPollEvents();// 处理窗口事件,轮询事件
        glfwSwapBuffers(m_Window);// 交换前后缓冲区
    }

    // 设置垂直同步
    void WindowsWindow::SetVSync(bool enabled) {
        if(enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }
    bool WindowsWindow::IsVSync() const {
        // 返回垂直同步状态的代码
        return m_Data.VSync;
    }

    void WindowsWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
    }
}	
