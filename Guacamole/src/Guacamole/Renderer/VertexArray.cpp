#include "gmpch.h"
#include "VertexArray.h"

#include "Guacamole/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Guacamole {

	Ref<VertexArray> VertexArray::Create()
	{
		switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: GM_CORE_ASSERT(false, "RendererAPI:None not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}