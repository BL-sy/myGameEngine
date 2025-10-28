#pragma once

#include "./Hazel/Window.h"
#include "./Hazel/log.h"

#include <GLFW/glfw3.h>

namespace Hazel {
    class HAZEL_API WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; };
        inline unsigned int GetHeight() const override { return m_Data.Height; };

        // 设置事件回调函数 
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
    private:
        void Init(const WindowProps& props);
        void SetCallbacks();
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;

        // 窗口数据结构体
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            // 是否启用垂直同步
            bool VSync;

            // 事件回调函数
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}

