#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "Window.h"

#include "Event/ApplicationEvent.h"

namespace Hazel {
    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application();

        void OnEvent(Event& e);

        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}

