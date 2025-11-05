#include "Hazel.h"

#include "hzpch.h"

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example") 
    {
    }
    void OnUpdate() override 
    {
        //HZ_INFO("ExampleLayer::Update");
        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
        {
            HZ_TRACE("A键按下(poll)");
        }
    }

    void OnEvent(Hazel::Event& event) override 
    {
        //HZ_TRACE("{0}",event.ToString());
        if(event.GetEventType() == Hazel::EventType::KeyPressed)
        {
            Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
            if (e.GetKeyCode() == HZ_KEY_A)
            {
                HZ_TRACE("A键按下(event)");
            }
        }
    }
};

class Sandbox : public Hazel::Application 
{
public:
    Sandbox() 
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Hazel::ImGuiLayer());
    }

    ~Sandbox() 
    {
    }

};

Hazel::Application* Hazel::CreateApplication() 
{
    return new Sandbox();
}