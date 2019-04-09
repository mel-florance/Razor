#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Razor {
	class Logger;

	class RAZOR_API Log
	{
	public:
		Log();
		static void init();
		
		std::function<void(const std::string&)> callback;

		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
		static std::shared_ptr<spdlog::logger> s_fileLogger;
		static Logger* s_editorLogger;

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }
		inline static std::shared_ptr<spdlog::logger>& getFileLogger() { return s_fileLogger; }
		inline static Logger* getEditorLogger() { return s_editorLogger; }

	};
}

#define RZ_TRACE(...) ::Razor::Log::getCoreLogger()->trace(__VA_ARGS__); 
#define RZ_INFO(...)  ::Razor::Log::getCoreLogger()->info(__VA_ARGS__); 
#define RZ_WARN(...)  ::Razor::Log::getCoreLogger()->warn(__VA_ARGS__);
#define RZ_ERROR(...) ::Razor::Log::getCoreLogger()->error(__VA_ARGS__); 
#define RZ_FATAL(...) ::Razor::Log::getCoreLogger()->critical(__VA_ARGS__);
