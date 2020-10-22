#pragma once

namespace Protocol {

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
}
