#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"

namespace Hazel {
    // �������Խṹ��
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Hazel Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {}
    };

    // ����ƽ̨�Ĵ��ڳ�����
    class HAZEL_API Window
    {
    public:
        // �¼��ص��������Ͷ���,�¼�ϵͳ���������ص��������������¼� 
        // ʹ��std::function��װһ������Event���õĺ���
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}
        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

		// �����¼��ص�����
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		// ���û���ô�ֱͬ��
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}	