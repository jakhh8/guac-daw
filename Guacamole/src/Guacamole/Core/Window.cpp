#include "gmpch.h"
#include "Window.h"

#ifdef GM_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#elif defined(GM_PLATFORM_LINUX)
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Guacamole {

    Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef GM_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
    #elif defined(GM_PLATFORM_LINUX)
		return CreateScope<WindowsWindow>(props);
	#else
		GM_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
    
}