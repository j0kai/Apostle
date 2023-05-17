#include "APpch.h"
#include "VertexArray.h"

#include "Apostle/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Apostle {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::API::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}

}