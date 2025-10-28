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

        // �����¼��ص����� 
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
    private:
        void Init(const WindowProps& props);
        void SetCallbacks();
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;

        // �������ݽṹ��
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            // �Ƿ����ô�ֱͬ��
            bool VSync;

            // �¼��ص�����
            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}

