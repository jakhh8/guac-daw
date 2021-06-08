#include "gmpch.h"
#include "Guacamole/Renderer/Framebuffer.h"

#include "Guacamole/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Guacamole {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:		GM_CORE_ASSERT(false, "RendererAPI::API::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLFramebuffer>(spec);
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}