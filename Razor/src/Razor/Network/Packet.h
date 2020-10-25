#pragma once

#include "Protocol.h"

#define MAX_USERNAME_LENGTH 20
#define MAX_PASSWORD_LENGTH 20
#define MAX_CLIENTS_LIST 64
#define MAX_TOKEN_LENGTH 64
#define MAX_GAMES_LIST 10
#define MAX_MESSAGE_LENGTH 50
#define MAX_CHANNEL_NAME_LENGTH 20
#define MAX_GAME_NAME_LENGTH 20
#define MAX_PLAYERS_LIST 10
#define MAX_GAME_MAP_LENGTH 20

namespace Razor {

	enum class PacketType : uint32_t {
		PING,
		PONG,
		LOGIN,
		GAME_INFO,
		GAMES_LIST,
		PLAYER_POSITION,
		CHAT_MESSAGE,
		LOGIN_RESPONSE,
		CLIENTS_LIST,
		CLIENT_INFO,
		CLIENT_JOINED,
		CLIENT_DISCONNECTED,
		CREATE_GAME,
		GAME_CREATED,
		GAME_DESTROYED,
		JOIN_GAME,
		LEAVE_GAME,
		REFRESH_GAMES_LIST,
		PLAYER_INFO,
		PLAYERS_LIST,
		PLAYER_READY,
		PLAYER_STRANGER_JOINED,
		PLAYER_SELF_JOINED,
		PLAYER_LEAVED,
		MARK_PLAYER_READY,
		NULL_PACKET
	};

	struct Packet
	{
		PacketType id;
		uint32_t size;
		char token[MAX_TOKEN_LENGTH];

		template<typename T>
		inline static T create(const char* token = nullptr) {
			T packet;
			packet.id = getId<T>();
			packet.size = sizeof(T);

			if (token != nullptr) {
				std::strncpy(packet.token, token, sizeof(Packet::token));
			}

			return packet;
		}

		template<typename T>
		inline static PacketType getId() {
			if constexpr (std::is_same_v<T, ChatMessage>)
				return PacketType::CHAT_MESSAGE;
			if constexpr (std::is_same_v<T, GameInfo>)
				return PacketType::GAME_INFO;
			if constexpr (std::is_same_v<T, Ping>)
				return PacketType::PING;
			if constexpr (std::is_same_v<T, Pong>)
				return PacketType::PONG;
			if constexpr (std::is_same_v<T, LoginRequest>)
				return PacketType::LOGIN;
			if constexpr (std::is_same_v<T, GamesList>)
				return PacketType::GAMES_LIST;
			if constexpr (std::is_same_v<T, LoginResponse>)
				return PacketType::LOGIN_RESPONSE;
			if constexpr (std::is_same_v<T, PlayerPosition>)
				return PacketType::PLAYER_POSITION;
			if constexpr (std::is_same_v<T, ClientsList>)
				return PacketType::CLIENTS_LIST;
			if constexpr (std::is_same_v<T, ClientInfo>)
				return PacketType::CLIENT_INFO;
			if constexpr (std::is_same_v<T, ClientJoined>)
				return PacketType::CLIENT_JOINED;
			if constexpr (std::is_same_v<T, ClientDisconnect>)
				return PacketType::CLIENT_DISCONNECTED;
			if constexpr (std::is_same_v<T, CreateGame>)
				return PacketType::CREATE_GAME;
			if constexpr (std::is_same_v<T, GameCreated>)
				return PacketType::GAME_CREATED;
			if constexpr (std::is_same_v<T, JoinGame>)
				return PacketType::JOIN_GAME;
			if constexpr (std::is_same_v<T, GameDestroyed>)
				return PacketType::GAME_DESTROYED;
			if constexpr (std::is_same_v<T, RefreshGamesList>)
				return PacketType::REFRESH_GAMES_LIST;
			if constexpr (std::is_same_v<T, PlayerInfo>)
				return PacketType::PLAYER_INFO;
			if constexpr (std::is_same_v<T, PlayersList>)
				return PacketType::PLAYERS_LIST;
			if constexpr (std::is_same_v<T, PlayerReady>)
				return PacketType::PLAYER_READY;
			if constexpr (std::is_same_v<T, PlayerStrangerJoined>)
				return PacketType::PLAYER_STRANGER_JOINED;
			if constexpr (std::is_same_v<T, PlayerSelfJoined>)
				return PacketType::PLAYER_SELF_JOINED;
			if constexpr (std::is_same_v<T, MarkPlayerReady>)
				return PacketType::MARK_PLAYER_READY;
			if constexpr (std::is_same_v<T, LeaveGame>)
				return PacketType::LEAVE_GAME;
			if constexpr (std::is_same_v<T, PlayerLeaved>)
				return PacketType::PLAYER_LEAVED;
		}

		inline std::string to_string() {
			switch (id) {
			default: return "NULL_PACKET";
			case PacketType::PING: return "PING";
			case PacketType::PONG: return "PONG";
			case PacketType::LOGIN: return "LOGIN";
			case PacketType::GAME_INFO: return "GAME_INFO";
			case PacketType::GAMES_LIST: return "GAMES_LIST";
			case PacketType::PLAYER_POSITION: return "PLAYER_POSITION";
			case PacketType::CHAT_MESSAGE: return "CHAT_MESSAGE";
			case PacketType::LOGIN_RESPONSE: return "LOGIN_RESPONSE";
			case PacketType::CLIENTS_LIST: return "CLIENTS_LIST";
			case PacketType::CLIENT_INFO: return "CLIENT_INFO";
			case PacketType::CLIENT_JOINED: return "CLIENT_JOINED";
			case PacketType::CLIENT_DISCONNECTED: return "CLIENT_DISCONNECTED";
			case PacketType::CREATE_GAME: return "CREATE_GAME";
			case PacketType::GAME_CREATED: return "GAME_CREATED";
			case PacketType::JOIN_GAME: return "JOIN_GAME";
			case PacketType::GAME_DESTROYED: return "GAME_DESTROYED";
			case PacketType::REFRESH_GAMES_LIST: return "REFRESH_GAMES_LIST";
			case PacketType::PLAYER_INFO: return "PLAYER_INFO";
			case PacketType::PLAYERS_LIST: return "PLAYERS_LIST";
			case PacketType::PLAYER_READY: return "PLAYER_READY";
			case PacketType::PLAYER_STRANGER_JOINED: return "PLAYER_STRANGER_JOINED";
			case PacketType::PLAYER_SELF_JOINED: return "PLAYER_SELF_JOINED";
			case PacketType::MARK_PLAYER_READY: return "MARK_PLAYER_READY";
			case PacketType::LEAVE_GAME: return "LEAVE_GAME";
			case PacketType::PLAYER_LEAVED: return "PLAYER_LEAVED";
			}
		}
	};

	//----------------------------------------
	// GENERAL
	//----------------------------------------

	struct Ping : public Packet {};
	struct Pong : public Packet {};

	//----------------------------------------
	// CLIENTS
	//----------------------------------------

	struct ClientInfo : public Packet {
		uint32_t userId;
		char username[MAX_USERNAME_LENGTH];
	};

	struct ClientJoined : public Packet {
		char username[MAX_USERNAME_LENGTH];
	};

	struct ClientDisconnect : public Packet {
		char username[MAX_USERNAME_LENGTH];
	};

	struct ClientsList : public Packet {
		ClientInfo clients[MAX_CLIENTS_LIST] = {};
	};


	//----------------------------------------
	// AUTHENTICATION
	//----------------------------------------

	struct LoginRequest : public Packet {
		char username[MAX_USERNAME_LENGTH];
		char password[MAX_PASSWORD_LENGTH];
	};

	struct LoginResponse : public Packet {
		Protocol::RequestStatus status;
		uint32_t userId = 0;
		char token[MAX_TOKEN_LENGTH];
	};

	//----------------------------------------
	// GAMES
	//----------------------------------------

	struct CreateGame : public Packet {
		char name[MAX_CHANNEL_NAME_LENGTH];
		int32_t max_players = MAX_PLAYERS_LIST;
		int32_t mode = 0;
		int32_t map = 0;
	};

	struct JoinGame : public Packet {
		uint32_t gameId = 0;
	};

	struct PlayerReady : public Packet {
		uint32_t userId = 0;
		bool ready = false;
	};

	struct MarkPlayerReady : public Packet {
		uint32_t gameId = 0;
		bool ready = false;
	};

	struct PlayerInfo : public Packet {
		uint32_t userId = 0;
		char username[MAX_USERNAME_LENGTH];
		bool ready = false;
	};

	struct PlayersList : public Packet {
		PlayerInfo players[MAX_PLAYERS_LIST] = {};
	};

	struct GameInfo : public Packet {
		uint32_t gameId = 0;
		uint32_t creator = 0;
		char name[MAX_GAME_NAME_LENGTH];
		uint32_t ping = 0;
		uint32_t mode = 0;
		uint32_t map = 0;
		uint32_t max_players = 10;
		uint32_t players_count = 0;
		PlayersList players;
		bool started = false;
	};

	struct PlayerStrangerJoined : public Packet {
		PlayerInfo info;
	};

	struct PlayerSelfJoined : public Packet {
		GameInfo info;
	};

	struct LeaveGame : public Packet {
		uint32_t gameId = 0;
	};

	struct PlayerLeaved : public Packet {
		PlayerInfo info;
	};

	struct RefreshGamesList : public Packet {
		uint32_t max_ping = 0;
		uint32_t maps[5];
		uint32_t min_players = 0;;
		uint32_t max_players = MAX_PLAYERS_LIST;
	};

	struct GamesList : public Packet {
		GameInfo games[MAX_GAMES_LIST] = {};
	};

	struct GameCreated : public Packet {
		GameInfo infos;
	};

	struct GameDestroyed : public Packet {
		uint32_t gameId;
	};

	//----------------------------------------
	// CHAT
	//----------------------------------------

	struct ChatMessage : public Packet {
		char username[MAX_USERNAME_LENGTH];
		char message[MAX_MESSAGE_LENGTH];
	};

	//----------------------------------------
	// IN GAME
	//----------------------------------------

	struct PlayerPosition : public Packet {
		float position[3] = { 0, 0, 0 };
	};

}
