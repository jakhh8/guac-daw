#include "gmpch.h"
#include "Texture.h"

#include "Guacamole/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Guacamole {

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: GM_CORE_ASSERT(false, "RendererAPI:None not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: GM_CORE_ASSERT(false, "RendererAPI:None not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}