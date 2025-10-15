#include "Hazel.h"

#include "hzpch.h"

class Sandbox : public Hazel::Application 
{
public:
	Sandbox() 
	{
	}

	~Sandbox() 
	{
	}

};

Hazel::Application* Hazel::CreateApplication() 
{
	return new Sandbox();
}