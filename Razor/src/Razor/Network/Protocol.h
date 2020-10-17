#pragma once

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
	CLIENT_DISCONNECTED
};

namespace Protocol {

	template<typename T>
	inline PacketType GetPacketId() {
		if constexpr (std::is_same_v<T, PacketChatMessage>)
			return PacketType::CHAT_MESSAGE;
		if constexpr (std::is_same_v<T, PacketGameInfo>)
			return PacketType::GAME_INFO;
		if constexpr (std::is_same_v<T, PacketPing>)
			return PacketType::PING;
		if constexpr (std::is_same_v<T, PacketPing>)
			return PacketType::PONG;
		if constexpr (std::is_same_v<T, PacketLoginRequest>)
			return PacketType::LOGIN;
		if constexpr (std::is_same_v<T, PacketGamesList>)
			return PacketType::GAMES_LIST;
		if constexpr (std::is_same_v<T, PacketLoginResponse>)
			return PacketType::LOGIN_RESPONSE;
		if constexpr (std::is_same_v<T, PacketPlayerPosition>)
			return PacketType::PLAYER_POSITION;
		if constexpr (std::is_same_v<T, PacketClientsList>)
			return PacketType::CLIENTS_LIST;
		if constexpr (std::is_same_v<T, PacketClientInfo>)
			return PacketType::CLIENT_INFO;
		if constexpr (std::is_same_v<T, PacketClientJoined>)
			return PacketType::CLIENT_JOINED;
		if constexpr (std::is_same_v<T, PacketClientJoined>)
			return PacketType::CLIENT_JOINED;
	}

	inline std::string to_string(PacketType type) {
		switch (type) {
		case PacketType::PING: return "PING";
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
		}
	}

}
