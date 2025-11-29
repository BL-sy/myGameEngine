#pragma once

constexpr char* acount_name = "BLsy";

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	int a = 5;
	HZ_INFO("Hello! {0}, Welcome to Hazel!", acount_name);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif