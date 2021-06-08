#include "gmpch.h"
#include "Buffer.h"

#include "Guacamole/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Guacamole {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:			GM_CORE_ASSERT(false, "RendererAPI::API::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(size);
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
    
    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:			GM_CORE_ASSERT(false, "RendererAPI::API::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch(RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:			GM_CORE_ASSERT(false, "RendererAPI::API::None is not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		GM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}