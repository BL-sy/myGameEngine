#pragma once

#include "Hazel/Core.h"

#include "Event.h"

namespace Hazel {

    // �����¼�����
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

    // ���̰����¼�
    class HAZEL_API KeyPressedEvent : public KeyEvent
    {
    public:
        // repeatCount ��ʾ�����ظ��Ĵ��������ڴ����������������
        KeyPressedEvent(int keycode, int repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount) {}
        inline int GetRepeatCount() const { return m_RepeatCount; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }
        // �궨�壬�����¼�������
        // ͨ���ú���Ի�ȡ�¼��ľ�̬���͡���̬���ͺ�����
        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    // �����ͷ��¼�
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