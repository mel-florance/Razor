#include "ChatController.h"
#include "../Interfaces/Chat.h"
#include "Razor/Network/Packet.h"
#include "../Client.h"
#include "MultiplayerController.h"

using namespace Razor;

std::vector<Razor::TCPClient::Message> ChatController::messages = {};

ChatController::ChatController(
	std::shared_ptr<Razor::TCPClient> client,
	const std::unordered_map<std::string, Razor::Texture*>& textures
) :
	Controller(client, State::UNDEFINED),
	input_message(""),
	scroll_to_bottom(false),
	last_message(nullptr)
{
	this->setInterface(new Chat(), textures);
}

ChatController::~ChatController()
{
}

void ChatController::OnEvent(Event& event)
{
}

void ChatController::OnUpdate(float delta)
{
}

void ChatController::OnRender()
{
	this->interface->render();
}

void ChatController::add_message(const std::string& username, const std::string& text, bool time)
{
	TCPClient::Message message;
	message.username = username;
	message.text = text;

	if (time) {
		tm* tm_local = Utils::getLocalTime();
		message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);
	}

	messages.push_back(message);
}

void ChatController::onMessage(Razor::Packet* packet)
{
	auto item = reinterpret_cast<ChatMessage*>(packet);
	tm* tm_local = Utils::getLocalTime();

	TCPClient::Message message;
	message.username = item->username;
	message.text = item->message;
	message.time = Utils::pad(tm_local->tm_hour) + ':' + Utils::pad(tm_local->tm_min);

	messages.push_back(message);
	Log::trace("Received: %s", std::string(item->message).c_str());
}

void ChatController::send()
{
	ChatMessage packet = Packet::create<ChatMessage>();
	std::strncpy(packet.message, input_message, sizeof(ChatMessage::message));
	client->emit(client->handle, packet);

	memset(input_message, 0, 128);
	scroll_to_bottom = true;
}
