#pragma once

#ifdef RZ_PLATFORM_WINDOWS

extern Razor::Application* Razor::createApplication();

int main(int argc, char** argv)
{
	auto app = Razor::createApplication();
	app->run();
	delete app;
}

#endif