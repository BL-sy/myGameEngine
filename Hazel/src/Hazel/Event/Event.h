#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"

namespace Hazel {
    // Hazel �е��¼���ǰ��������״̬������ζ�ŵ��¼�����ʱ
    // ���������Ȳ��ұ�����������
    // ����δ�������õĲ��Կ��������¼������л����¼�
    // �ڸ��½׶εġ��¼������ֶ����ǽ����ŶӺʹ���

    // �¼�����������
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // ���ڶ��¼����з����λ���룬���Խ��¼�����Ϊ������
    // ���ڿ��ٹ����¼�
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

// �궨�����ڼ��¼������ͺ������
// ��̬���������ڲ�����ʵ��������»�ȡ�¼�����
// ��̬�������ڻ�ȡʵ�����¼�����
// �������ڵ��Ժ���־��¼
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

        // �ж��¼��Ƿ�����ĳ�����
        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    protected:
        // �ж��¼��Ƿ񱻴���
        // �¼��������Ͳ����ٴ��ݸ�������
        bool Handled = false;
    };

    // �¼��ַ������������ͽ��¼��ַ�����Ӧ�Ĵ�����
    class HAZEL_API EventDispatcher
    {
        template<typename T>
        // ����һ���������ͱ�������ʾ�����¼����ò����ز���ֵ�ĺ�������
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

    // �������������������ӡ�¼���Ϣ
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}