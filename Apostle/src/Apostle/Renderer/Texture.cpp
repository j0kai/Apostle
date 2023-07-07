#include "APpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Apostle {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}
	Ref<Texture2D> Apostle::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetRenderAPI())
		{
			case RendererAPI::API::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}


}

