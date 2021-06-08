#pragma once

#include <memory>

#include <Guacamole/Core/PlatformDetection.h>

#ifdef GM_DEBUG
	#if defined(GM_PLATFORM_WINDOWS)
		#define GM_DEBUGBREAK() __debugbreak()
	#elif defined(GM_PLATFORM_LINUX)
		#include <signal.h>
		#define GM_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define GM_ENABLE_ASSERTS
#else
	#define GM_DEBUGBREAK()
#endif

#define GM_EXPAND_MACRO(x) x
#define GM_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define GM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Guacamole {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	
}

#include "Guacamole/Core/Log.h"
#include "Guacamole/Core/Assert.h"