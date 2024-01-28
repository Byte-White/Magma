#ifndef MAGMA_LOGGING_H
#define MAGMA_LOGGING_H
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <memory>

namespace mg
{

	class Log
	{
	public:
		static void Init(const std::string& pattern = "%^[%T] %n: %v%$");

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }


		//CORE LOG FNS

		template<typename... Args>
		inline static void CoreTrace(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{

			GetCoreLogger()->trace(fmt, args...);
		}

		template<typename... Args>
		inline static void CoreInfo(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetCoreLogger()->info(fmt, args...);
		}
		template<typename... Args>
		inline static void CoreWarn(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetCoreLogger()->warn(fmt, args...);
		}
		template<typename... Args>
		inline static void CoreError(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetCoreLogger()->error(fmt, args...);
		}
		template<typename... Args>
		inline static void CoreCritical(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetCoreLogger()->critical(fmt, args...);
		}

		template<typename... Args>
		inline static void CoreDebug(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetCoreLogger()->debug(fmt, args...);
		}


		//CLIENT LOG FNS
		template<typename... Args>
		inline static void Trace(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetClientLogger()->trace(fmt, args...);
		}

		template<typename... Args>
		inline static void Info(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetClientLogger()->info(fmt, args...);
		}
		template<typename... Args>
		inline static void Warn(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetClientLogger()->warn(fmt, args...);
		}
		template<typename... Args>
		inline static void Error(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetClientLogger()->error(fmt, args...);
		}
		template<typename... Args>
		inline static void Critical(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetClientLogger()->critical(fmt, args...);
		}

		template<typename... Args>
		inline static void Debug(spdlog::format_string_t<Args...> fmt, Args &&... args)
		{
			GetClientLogger()->debug(fmt, args...);
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
//define MAGMA_NO_LOGGING to disable all logging macros

#ifdef MAGMA_NO_LOGGING
#define MAGMA_NO_CORE_LOGGING
#define MAGMA_NO_CLIENT_LOGGING
#endif
#ifndef MAGMA_NO_CORE_LOGGING
	// Core log macros
#define MAGMA_CORE_TRACE(...)		mg::Log::CoreTrace(__VA_ARGS__)
#define MAGMA_CORE_INFO(...)			mg::Log::CoreInfo(__VA_ARGS__)
#define MAGMA_CORE_WARN(...)			mg::Log::CoreWarn(__VA_ARGS__)
#define MAGMA_CORE_ERROR(...)		mg::Log::CoreError(__VA_ARGS__)
#define MAGMA_CORE_CRITICAL(...)		mg::Log::CoreCritical(__VA_ARGS__)
#define MAGMA_CORE_DEBUG(...)		mg::Log::CoreDebug(__VA_ARGS__)
#else
#define MAGMA_CORE_TRACE(...)
#define MAGMA_CORE_INFO(...)
#define MAGMA_CORE_WARN(...)
#define MAGMA_CORE_ERROR(...)
#define MAGMA_CORE_CRITICAL(...)
#define MAGMA_CORE_DEBUG(...)		
#endif

#ifndef MAGMA_NO_CLIENT_LOGGING
	// Client log macros
#define MAGMA_TRACE(...)				mg::Log::Trace(__VA_ARGS__)
#define MAGMA_INFO(...)				mg::Log::Info(__VA_ARGS__)
#define MAGMA_WARN(...)				mg::Log::Warn(__VA_ARGS__)
#define MAGMA_ERROR(...)				mg::Log::Error(__VA_ARGS__)
#define MAGMA_CRITICAL(...)			mg::Log::Critical(__VA_ARGS__)
#define MAGMA_DEBUG(...)				mg::Log::Debug(__VA_ARGS__)
#else
#define MAGMA_TRACE(...)
#define MAGMA_INFO(...)
#define MAGMA_WARN(...)
#define MAGMA_ERROR(...)
#define MAGMA_CRITICAL(...)
#define MAGMA_DEBUG(...)			
#endif

#endif