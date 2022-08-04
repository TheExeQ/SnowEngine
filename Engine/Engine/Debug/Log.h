#pragma once
#include "Engine/Core/Base.h"

#include <spdlog/spdlog.h>
#include <string>

namespace Snow
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define CORE_LOG_TRACE(...)    ::Snow::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...)     ::Snow::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)     ::Snow::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...)    ::Snow::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...) ::Snow::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...)         ::Snow::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::Snow::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::Snow::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::Snow::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::Snow::Log::GetClientLogger()->critical(__VA_ARGS__)