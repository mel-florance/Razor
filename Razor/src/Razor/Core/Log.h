#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Razor 
{

	class Logger;

	class Log
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

		static void trace(const char* format, ...);
		static void info(const char* format, ...);
		static void warn(const char* format, ...);
		static void error(const char* format, ...);
		static void fatal(const char* format, ...);
	};

}
