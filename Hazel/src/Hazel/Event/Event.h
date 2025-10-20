#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"

namespace Hazel {
    // Hazel 中的事件当前处于阻塞状态，这意味着当事件发生时
    // 立即被调度并且必须立即处理。
    // 对于未来，更好的策略可能是在事件总线中缓冲事件
    // 在更新阶段的“事件”部分对它们进行排队和处理。

    // 事件的所有类型
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // 用于对事件进行分类的位掩码，可以将事件归类为多个类别
    // 用于快速过滤事件
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

// 宏定义用于简化事件的类型和类别定义
// 静态类型用于在不创建实例的情况下获取事件类型
// 动态类型用于获取实例的事件类型
// 名称用于调试和日志记录
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class HAZEL_API Event
    {
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        // 判断事件是否属于某个类别
        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    protected:
        // 判断事件是否被处理
        // 事件被处理后就不会再传递给其他层
        bool Handled = false;
    };

    // 事件分发器，根据类型将事件分发给相应的处理函数
    class HAZEL_API EventDispatcher
    {
        template<typename T>
        // 定义一个函数类型别名，表示接受事件引用并返回布尔值的函数对象
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event)
            : m_Event(event)
        {
        }

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

    // 重载输出运算符，方便打印事件信息
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}