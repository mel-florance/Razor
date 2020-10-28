#include "LoadingController.h"
#include "../Interfaces/Loading.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"
#include "LobbyController.h"
#include "Editor/Components/AssetsManager.h"
#include "MultiplayerController.h"
#include "Razor/Scene/ScenesManager.h"
#include "Razor/Materials/Presets/PbrMaterial.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Core/Engine.h"

using namespace Razor;

static std::mutex nodes_mutex;

LoadingController::LoadingController(
	std::shared_ptr<Razor::TCPClient> client,
	const std::unordered_map<std::string, Razor::Texture*>& textures
) :
	Controller(client, State::LOADING),
	progress(0.0f),
	started(false),
	finished(false)
{
	this->setInterface(new Loading(), textures);
}

LoadingController::~LoadingController()
{
}

void LoadingController::OnEvent(Event& event)
{

}

void LoadingController::OnUpdate(float delta)
{
	if (started) {
		// Get loading informations from the lobby (map etc...)
		auto lobby = TestLayer::getController<LobbyController>("lobby");
		std::string map_name = MultiplayerController::get_map_name(lobby->current_game_infos.map);

		// Load the map
		Node* result = nullptr;
		auto am = new Razor::AssetsManager(nullptr);

		am->import(result, [=](Node* node) {

			// Add & setup models to the main scene.
			std::shared_ptr<Node> n = std::make_shared<Node>(node);
			n->active = true;
			n->transform.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
			n->transform.setRotation(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f));
			n->setupMeshBuffers(n);
			Application::Get().getScenesManager()->getActiveScene()->getSceneGraph()->addNode(n);

			finished = true;
			std::cout << "loaded map" << std::endl;

			// Change state pour Game.
			Razor::TestLayer::current_state = Controller::State::GAME;

		}, Variant("./data/sponza.obj"));

		started = false;

		//progress += delta / 100;
	}
}

void LoadingController::OnRender()
{
	this->interface->render();
}
