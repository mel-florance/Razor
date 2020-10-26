#include "Client.h"

#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Network/Packet.h"

#include "Controllers/LobbyController.h"
#include "Controllers/MultiplayerController.h"
#include "Controllers/ChatController.h"
#include "Controllers/OptionsController.h"
#include "Controllers/HomeController.h"

namespace Razor {

	Client::Client() :
		Application(false, Application::Type::CLIENT, WindowProps("Razor Client"))
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

	Controller::State TestLayer::current_state = Controller::State::MAIN_MENU;
	std::unordered_map<std::string, std::shared_ptr<Controller>> TestLayer::controllers = {};

	TestLayer::TestLayer(Razor::ScenesManager* sceneManager, Razor::Engine* engine) :
		Layer("Test"),
		sm(sceneManager),
		engine(engine),
		connected(false),
		input_host("127.0.0.1"),
		input_port("55555"),
		input_message(""),
		input_name("Guest"),
		main_menu_button_size(ImVec2(240, 0))
	{
		ui_textures["logo"] = new Razor::Texture("./data/logo.png", true);
		ui_textures["background"] = new Razor::Texture("./data/background.jpg", true);

		client = std::make_shared<TCPClient>();

		controllers["lobby"] = std::make_shared<LobbyController>(client, ui_textures);
		controllers["multiplayer"] = std::make_shared<MultiplayerController>(client, ui_textures);
		controllers["chat"] = std::make_shared<ChatController>(client, ui_textures);
		controllers["options"] = std::make_shared<OptionsController>(client, ui_textures);
		controllers["home"] = std::make_shared<HomeController>(client, ui_textures);

		client->bind(PacketType::LOGIN_RESPONSE, &MultiplayerController::onLoginResponse);
		client->bind(PacketType::GAME_CREATED, &LobbyController::onGameCreated);
		client->bind(PacketType::GAMES_LIST, &MultiplayerController::onGamesList);
		client->bind(PacketType::GAME_DESTROYED, &MultiplayerController::onGameDestroyed);
		client->bind(PacketType::PLAYER_READY, &LobbyController::onPlayerReady);
		client->bind(PacketType::PLAYER_STRANGER_JOINED, &LobbyController::onPlayerStrangerJoined);
		client->bind(PacketType::PLAYER_SELF_JOINED, &LobbyController::onPlayerSelfJoined);
		client->bind(PacketType::PLAYER_LEAVED, &LobbyController::onPlayerLeaved);
		client->bind(PacketType::CHAT_MESSAGE, &ChatController::onMessage);
	}

	void TestLayer::OnUpdate(float delta)
	{
		for (auto it = controllers.begin(); it != controllers.end(); ++it) {
			if (it->second->state == current_state)
				it->second->OnUpdate(delta);
		}

	}

	void TestLayer::OnAttach()
	{

	}

	void TestLayer::OnEvent(Razor::Event& event) {
		for (auto it = controllers.begin(); it != controllers.end(); ++it) {
			if (it->second->state == current_state)
				it->second->OnEvent(event);
		}
	}

	void TestLayer::OnImGuiRender()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		bool p_open;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_DockNodeHost |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Razor Client", &p_open, window_flags);
		ImGui::PopStyleVar();

		for (auto it = controllers.begin(); it != controllers.end(); ++it) {
			if (it->second->state == current_state)
				it->second->OnRender();
		}

		ImGui::End();
	}
}
