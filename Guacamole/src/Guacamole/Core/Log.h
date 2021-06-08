#pragma once

#include "Guacamole/Core/Base.h"
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Guacamole {
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define GM_CORE_TRACE(...) ::Guacamole::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GM_CORE_INFO(...)  ::Guacamole::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GM_CORE_WARN(...)  ::Guacamole::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GM_CORE_ERROR(...) ::Guacamole::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GM_CORE_FATAL(...) ::Guacamole::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define GM_TRACE(...)      ::Guacamole::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GM_INFO(...)       ::Guacamole::Log::GetClientLogger()->info(__VA_ARGS__)
#define GM_WARN(...)       ::Guacamole::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GM_ERROR(...)      ::Guacamole::Log::GetClientLogger()->error(__VA_ARGS__)
#define GM_FATAL(...)      ::Guacamole::Log::GetClientLogger()->fatal(__VA_ARGS__)