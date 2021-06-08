#include "gmpch.h"
#include "Guacamole/Renderer/GraphicsContext.h"

#include "Guacamole/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Guacamole {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    GM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}