#pragma once

#include "Guacamole/Core/Base.h"
#include "Guacamole/Core/Log.h"

#include <filesystem>

#ifdef GM_ENABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define GM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { GM##type##ERROR(msg, __VA_ARGS__); GM_DEBUGBREAK(); } }
	#define GM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) GM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define GM_INTERNAL_ASSERT_NO_MSG(type, check) GM_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", GM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define GM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define GM_INTERNAL_ASSERT_GET_MACRO(...) GM_EXPAND_MACRO( GM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, GM_INTERNAL_ASSERT_WITH_MSG, GM_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define GM_ASSERT(...) GM_EXPAND_MACRO( GM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define GM_CORE_ASSERT(...) GM_EXPAND_MACRO( GM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define GM_ASSERT(...)
	#define GM_CORE_ASSERT(...)
#endif