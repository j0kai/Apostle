#include "APpch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Apostle {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}

}