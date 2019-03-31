#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Razor {

	class RZ_API Log
	{
	public:
		static void init();
		
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
		static std::shared_ptr<spdlog::logger> s_fileLogger;

		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }
		inline static std::shared_ptr<spdlog::logger>& getFileLogger() { return s_fileLogger; }
	};

}

// Core log macros
#define RZ_CORE_TRACE(...) ::Razor::Log::getCoreLogger()->trace(__VA_ARGS__)
#define RZ_CORE_INFO(...)  ::Razor::Log::getCoreLogger()->info(__VA_ARGS__)
#define RZ_CORE_WARN(...)  ::Razor::Log::getCoreLogger()->warn(__VA_ARGS__)
#define RZ_CORE_ERROR(...) ::Razor::Log::getCoreLogger()->error(__VA_ARGS__)
#define RZ_CORE_FATAL(...) ::Razor::Log::getCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define RZ_TRACE(...) ::Razor::Log::getClientLogger()->trace(__VA_ARGS__)
#define RZ_INFO(...)  ::Razor::Log::getClientLogger()->info(__VA_ARGS__)
#define RZ_WARN(...)  ::Razor::Log::getClientLogger()->warn(__VA_ARGS__)
#define RZ_ERROR(...) ::Razor::Log::getClientLogger()->error(__VA_ARGS__)
#define RZ_FATAL(...) ::Razor::Log::getClientLogger()->fatal(__VA_ARGS__)

// Client log macros
#define RZ_FILE_TRACE(...) Razor::Log::getFileLogger()->trace(__VA_ARGS__); Razor::Log::getFileLogger()->flush_on(spdlog::level::trace)
#define RZ_FILE_INFO(...) Razor::Log::getFileLogger()->info(__VA_ARGS__); Razor::Log::getFileLogger()->flush_on(spdlog::level::info)
#define RZ_FILE_WARN(...) Razor::Log::getFileLogger()->warn(__VA_ARGS__); Razor::Log::getFileLogger()->flush_on(spdlog::level::warn)
#define RZ_FILE_ERROR(...) Razor::Log::getFileLogger()->error(__VA_ARGS__); Razor::Log::getFileLogger()->flush_on(spdlog::level::info)
#define RZ_FILE_FATAL(...) Razor::Log::getFileLogger()->fatal(__VA_ARGS__); Razor::Log::getFileLogger()->flush_on(spdlog::level::fatal)
