#pragma once

#include "Hazel/Core.h"

#include "Event.h"

namespace Hazel {

    // 键盘事件基类
    class HAZEL_API KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(int keycode)
            : m_KeyCode(keycode) {}
        int m_KeyCode;
    };

    // 键盘按下事件
    class HAZEL_API KeyPressedEvent : public KeyEvent
    {
    public:
        // repeatCount 表示按键重复的次数（用于处理按键长按的情况）
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount) {}
        inline int GetRepeatCount() const { return m_RepeatCount; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }
        // 宏定义，定义事件的类型
        // 通过该宏可以获取事件的静态类型、动态类型和名称
        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    // 键盘释放事件
    class HAZEL_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            : KeyEvent(keycode) {}
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}