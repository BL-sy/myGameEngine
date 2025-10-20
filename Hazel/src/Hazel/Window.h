#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"

namespace Hazel {
    // 窗口属性结构体
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

    // 基于平台的窗口抽象类
    class HAZEL_API Window
    {
    public:
        // 事件回调函数类型定义,事件系统会调用这个回调函数来处理窗口事件 
        // 使用std::function封装一个接受Event引用的函数
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}
        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

		// 设置事件回调函数
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		// 启用或禁用垂直同步
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}	