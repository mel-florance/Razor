#pragma once

#include "Razor/Application/Application.h"
#include "Razor/ImGui/ImGuiLayer.h"
#include "Razor/Network/TCPClient.h"

namespace Razor {

	class Client : public Razor::Application
	{
	public:
		Client();
		~Client();
	};

	class TestLayer : public Razor::Layer
	{
	public:
		TestLayer(Razor::ScenesManager* sceneManager, Razor::Engine* engine);

		void OnUpdate(float delta);
		void OnAttach();
		void OnImGuiRender();
		void OnEvent(Razor::Event& event);

		Razor::ScenesManager* sm;
		Razor::Engine* engine;

		std::unique_ptr<TCPClient> client;
		Texture* logo;

		bool connected;
		char input_host[15];
		char input_port[6];
		char input_message[128];
	};

}