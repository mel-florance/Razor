#pragma once

#include "Razor/Application/Application.h"
#include "Razor/ImGui/ImGuiLayer.h"
#include "Razor/Network/TCPClient.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "Controller.h"

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

		void Send();
		void SetPlayerReady();

		void display_connect();
		void display_game();
		void display_main_menu();
		void display_options();

		template<typename T>
		static inline std::shared_ptr<T> getController(const std::string& name) {
			auto it = controllers.find(name);

			if (it != controllers.end()) {
				return std::dynamic_pointer_cast<T>(it->second);
			}

			return nullptr;
		}

		static Controller::State current_state;
		static std::unordered_map<std::string, std::shared_ptr<Controller>> controllers;

		Razor::ScenesManager* sm;
		Razor::Engine* engine;

		std::shared_ptr<TCPClient> client;
		std::unordered_map<std::string, Texture*> ui_textures;

		bool connected;
		char input_host[15];
		char input_port[6];

		char input_name[20];
		char input_message[128];

		bool scrollToBottom;
		TCPClient::Message* lastMessage;
	
		ImVec2 main_menu_button_size;
	};

}