#include "rzpch.h"

#include "Log.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Razor {

	RAZOR_API std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	RAZOR_API std::shared_ptr<spdlog::logger> Log::s_clientLogger;
	RAZOR_API std::shared_ptr<spdlog::logger> Log::s_fileLogger;

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
}
