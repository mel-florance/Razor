#pragma once

namespace Razor
{

	class Http
	{
	public:
		Http();
		~Http();

		enum class RequestType
		{
			GET,
			POST
		};

		static std::string get(const std::string& url, const std::string& endpoint = "/");
		static std::string requestTypeToStr(RequestType type);
		static std::string formatRequestHeader(const std::string& url, const std::string& endpoint, RequestType type = RequestType::GET);
	};

}
