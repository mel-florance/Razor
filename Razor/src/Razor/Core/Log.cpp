#include "rzpch.h"

#include "Log.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "Editor/Components/Logger.h"

namespace Razor {

	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;
	std::shared_ptr<spdlog::logger> Log::s_fileLogger;
	Logger* Log::s_editorLogger;

	Log::Log()
	{
		
	}

	void Log::init()
	{
		spdlog::init_thread_pool(10000, 1);

		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_fileLogger = spdlog::basic_logger_mt<spdlog::async_factory>("Razor", "Razor.log");
		s_fileLogger->set_level(spdlog::level::trace);

		s_coreLogger = spdlog::stdout_color_mt("RAZOR");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);

		spdlog::set_default_logger(s_fileLogger);
	}

	void Log::trace(const char* format, ...)
	{
		va_list args;
		char buf[1024];
		va_start(args, format);
		vsnprintf(buf, IM_ARRAYSIZE(buf), format, args);
		s_coreLogger->trace(buf);

		if(s_editorLogger != nullptr)
			s_editorLogger->addLog(std::string(buf));

		va_end(args);
	}

	void Log::info(const char* format, ...)
	{
		va_list args;
		char buf[1024];
		va_start(args, format);
		vsnprintf(buf, IM_ARRAYSIZE(buf), format, args);
		s_coreLogger->info(buf);

		if (s_editorLogger != nullptr)
			s_editorLogger->addLog(std::string(buf));

		va_end(args);
	}

	void Log::warn(const char * format, ...)
	{
		va_list args;
		char buf[1024];
		va_start(args, format);
		vsnprintf(buf, IM_ARRAYSIZE(buf), format, args);
		s_coreLogger->warn(buf);

		if (s_editorLogger != nullptr)
			s_editorLogger->addLog(std::string(buf));

		va_end(args);
	}

	void Log::error(const char * format, ...)
	{
		va_list args;
		char buf[1024];
		va_start(args, format);
		vsnprintf(buf, IM_ARRAYSIZE(buf), format, args);
		s_coreLogger->error(buf);

		if (s_editorLogger != nullptr)
			s_editorLogger->addLog(std::string(buf));

		va_end(args);
	}

	void Log::fatal(const char * format, ...)
	{
		va_list args;
		char buf[1024];
		va_start(args, format);
		vsnprintf(buf, IM_ARRAYSIZE(buf), format, args);
		s_coreLogger->critical(buf);

		if (s_editorLogger != nullptr)
			s_editorLogger->addLog(std::string(buf));

		va_end(args);
	}

}
