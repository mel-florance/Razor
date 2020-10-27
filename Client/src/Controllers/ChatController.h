#pragma once

#include "../Controller.h"
#include "../Interfaces/Chat.h"

namespace Razor {
	class Texture;
}

class ChatController : public Controller
{
public:
	ChatController(
		std::shared_ptr<Razor::TCPClient> client,
		const std::unordered_map<std::string, Razor::Texture*>& textures
	);
	~ChatController();

	void OnEvent(Razor::Event& event) override;
	void OnUpdate(float delta) override;
	void OnRender() override;

	static void add_message(const std::string& username, const std::string& text, bool time = true);

	static void onMessage(Razor::Packet* packet);

	void send();

	char input_message[128];
	bool scroll_to_bottom;
	static std::vector<Razor::TCPClient::Message> messages;
	Razor::TCPClient::Message* last_message;
};

