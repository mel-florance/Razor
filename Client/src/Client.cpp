#include "Client.h"

#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Scene/ScenesManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Razor {

	Client::Client() :
		Application(false, Application::Type::CLIENT, WindowProps("Razor Client", 800.0f, 600.0f))
	{
		Razor::ScenesManager* sm = this->getScenesManager();
		Razor::Engine* engine = this->getEngine();
		auto camera = new FPSCamera(&Application::Get().GetWindow());

		auto main = new ImGuiLayer(nullptr);

		auto layer = new TestLayer(sm, engine);
		PushLayer(layer);
		PushOverlay(main);

		auto scene = sm->getActiveScene();
		scene->addCamera(camera);
		scene->setActiveCamera(camera);
	}

	Client::~Client()
	{
	}

	TestLayer::TestLayer(Razor::ScenesManager* sceneManager, Razor::Engine* engine) :
		Layer("Test"),
		sm(sceneManager),
		engine(engine),
		connected(false),
		input_host("127.0.0.1"),
		input_port("55555"),
		input_message(""),
		input_name("Guest"),
		scrollToBottom(false),
		current_state(MenuState::CONNECT)
	{
		client = std::make_unique<TCPClient>();

		logo = new Razor::Texture("./data/logo.png", true);

		//tutorial::AddressBook book;
	}

	void TestLayer::OnUpdate(float delta)
	{

	}

	void TestLayer::OnAttach()
	{

	}

	void TestLayer::OnEvent(Razor::Event& event) {

	}

	void TestLayer::Send() {
		PacketChatMessage packet;
		std::strncpy(packet.message, input_message, sizeof(PacketChatMessage::message));
		packet.id = PacketType::CHAT_MESSAGE;
		packet.size = sizeof(PacketChatMessage);
		client->emit(client->handle, packet);

		memset(input_message, 0, 128);
		scrollToBottom = true;
	}

	void TestLayer::Login() {
		PacketLoginRequest packet;
		std::strncpy(packet.username, input_name, sizeof(PacketLoginRequest::username));
		std::strncpy(packet.password, "test", sizeof(PacketLoginRequest::password));
		packet.id = PacketType::LOGIN;
		packet.size = sizeof(PacketLoginRequest);
		client->emit(client->handle, packet);
	}

	void TestLayer::display_connect()
	{
		ImGui::Dummy(ImVec2(-1, 20));
		unsigned int logo_size = 128;

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.5f) - logo_size * 0.5f);
		ImGui::Image((void*)logo->getId(), ImVec2(logo_size, logo_size), ImVec2(), ImVec2(1, -1));
		ImGui::Dummy(ImVec2(-1, 20));

		std::string text = "Welcome to the razor client!";
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() * 0.5f) - ImGui::CalcTextSize(text.c_str()).x * 0.5f);

		ImGui::Text("%s", text.c_str());
		ImGui::Dummy(ImVec2(-1, 20));

		//auto indent = (ImGui::GetWindowSize().x - 380) / 2;
		//ImGui::Indent(indent);

		if (!connected) {
			ImGui::Text("Name ");
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##nale", input_name, 20);
			ImGui::PopItemWidth();
			ImGui::Text("Host ");
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##ipaddr", input_host, 15);
			ImGui::PopItemWidth();
			ImGui::Text(" Port ");
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##port", input_port, 6);
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(-1, 10));

			if (ImGui::Button("Connect", ImVec2(-1, 0))) {
				connected = client->Connect(input_host, std::stoi(input_port));

				if (connected) {
					Login();
				}
			}
		}
		else {
			ImGui::Text("Connected to %s:%d as %s", input_host, std::stoi(input_port), input_name);

			if (ImGui::Button("Disconnect", ImVec2(-1, 0))) {
				client->Messages.clear();
				client->Clients.clear();
				client->Close();
				connected = false;
			}

			ImGui::Columns(2, "chat_area");
			ImGui::SetColumnWidth(0, 120);

			ImGui::BeginChild("users", { 0, 180 }, false);

			for (auto& user : client->Clients) {
				if (user.size() > 0) {
					ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), user.c_str());
				}
			}

			ImGui::End();

			ImGui::NextColumn();
			ImGui::BeginChild("messages", { 0, 180 }, false);

			for (auto& message : client->Messages) {
				if (message.text.size() > 0) {
					ImGui::TextColored(ImVec4(0.4f, 0.4f, 0.4f, 4.0f), message.time.c_str());
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 5.0f), message.username.c_str());
					ImGui::SameLine();
					ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), message.text.c_str());
				}
			}

			if (client->Messages.size() > 0) {
				auto last = &client->Messages.back();

				if (last != NULL) {
					if (lastMessage != last) {
						ImGui::SetScrollHere(0.f);
						lastMessage = last;
					}
				}
			}

			ImGui::EndChild();
			ImGui::PushItemWidth(ImGui::GetItemRectSize().x - 85);

			if (ImGui::InputText("##message", input_message, 128, ImGuiInputTextFlags_EnterReturnsTrue)) {
				if (strlen(input_message) > 0) {
					Send();
				}

				ImGui::SetKeyboardFocusHere(0);
			}

			ImGui::PopItemWidth();
			ImGui::SameLine();

			if (ImGui::Button("Send", ImVec2(80, 0))) {
				if (strlen(input_message) > 0) {
					Send();
				}

				ImGui::SetKeyboardFocusHere(0);
			}

			ImGui::Columns(1);
		}

		//PacketGamesList list;
		//list.id = PacketType::GAMES_LIST;
		//list.size = sizeof(PacketGamesList);

		//PacketGameInfo info;
		//info.gameId = 321413;
		//info.ping = 25;
		//info.size = sizeof(PacketGameInfo);
		//info.players = 6;

		//PacketGameInfo game2;
		//game2.gameId = 777;
		//game2.ping = 48;
		//game2.size = sizeof(PacketGameInfo);
		//game2.players = 55;

		//list.games[0] = game2;
		//list.games[1] = info;

		//state = send(sock, reinterpret_cast<char*>(&list), sizeof(PacketGamesList), 0);

		//PacketPlayerPosition player;
		//player.id = PacketType::PLAYER_POSITION;
		//player.size = sizeof(PacketPlayerPosition);
		//glm::vec3 pos = { 511.0f, 512.0f, 321.0f };

		//std::memcpy(player.position, glm::value_ptr(pos), sizeof(PacketPlayerPosition::position));

		//ImGui::Indent(-indent);
	}

	void TestLayer::display_game()
	{

	}

	void TestLayer::OnImGuiRender()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		bool p_open;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_DockNodeHost | ImGuiWindowFlags_NoResize;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 0.0f));
		ImGui::Begin("Razor Client", &p_open, window_flags);
		ImGui::PopStyleVar();

		switch (current_state) {
		case MenuState::CONNECT:
			display_connect();
		case MenuState::INGAME:
			display_game();
		}

		ImGui::End();
	}
}