#include "APpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Apostle {

	Ref<Texture2D> Apostle::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetRenderAPI())
		{
			case RendererAPI::API::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}


}

