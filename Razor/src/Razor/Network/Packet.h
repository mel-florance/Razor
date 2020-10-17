#pragma once

#include "Protocol.h"

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_CLIENTS_LIST 64
#define MAX_TOKEN_LENGTH 64
#define MAX_GAMES_LIST 10
#define MAX_MESSAGE_LENGTH 50

struct Packet 
{
	PacketType id;
	uint32_t size;
	char token[MAX_TOKEN_LENGTH];

	template<typename T>
	static T create(const char* token = nullptr) {
		T packet;
		packet.id = Protocol::GetPacketId<T>();
		packet.size = sizeof(T);

		if (token != nullptr) {
			std::strncpy(packet.token, token, sizeof(Packet::token));
		}

		return packet;
	}
};

enum class RequestStatus : uint32_t {
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	PARTIAL = 206,
	BAD_REQUEST = 400,
	UNAUTHORIZED = 401,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	TEAPOT = 418,
	TOO_MANY_REQUESTS = 429,
	CONNECTED = 200,
	LOGGED = 205
};

enum class ResponseStatus : uint32_t {
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED = 501,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503
};

struct PacketPing : public Packet {};
struct PacketPong : public Packet {};

struct PacketClientInfo : public Packet {
	char username[MAX_USERNAME_LENGTH];
};

struct PacketClientsList : public Packet {
	PacketClientInfo clients[MAX_CLIENTS_LIST] = {};
};

struct PacketLoginRequest : public Packet {
	char username[MAX_USERNAME_LENGTH];
	char password[MAX_PASSWORD_LENGTH];
};

struct PacketClientJoined : public Packet {
	char username[MAX_USERNAME_LENGTH];
};

struct PacketClientDisconnect : public Packet {
	char username[MAX_USERNAME_LENGTH];
};

struct PacketLoginResponse : public Packet{
	RequestStatus status;
	char message[MAX_MESSAGE_LENGTH];
	char token[MAX_TOKEN_LENGTH];
	PacketClientsList clients;
};

struct PacketGameInfo : public Packet {
	unsigned int gameId = 0;
	unsigned int ping = 0;
	unsigned int players = 0;
};

struct PacketGamesList : public Packet {
	PacketGameInfo games[MAX_GAMES_LIST] = {};
};

struct PacketPlayerPosition : public Packet {
	float position[3] = {0, 0, 0};
};

struct PacketChatMessage : public Packet {
	char username[MAX_USERNAME_LENGTH];
	char message[MAX_MESSAGE_LENGTH];
};