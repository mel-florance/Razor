#pragma once

#ifdef RZ_PLATFORM_WINDOWS

extern Razor::Application* Razor::createApplication();

int main(int argc, char** argv)
{
	Razor::Log::init();
	RZ_CORE_WARN("Log system initialized.");
	RZ_INFO("Application started");

	auto app = Razor::createApplication();
	app->run();
	delete app;


}

#endif