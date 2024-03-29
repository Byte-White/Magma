#include "Magma/Core/Logging.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace mg
{

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(const std::string& pattern)
	{
		spdlog::set_pattern(pattern);
		s_CoreLogger = spdlog::stdout_color_mt("MAGMA");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}