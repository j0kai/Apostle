#include "APpch.h"
#include "VertexArray.h"

#include "Apostle/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Apostle {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetRenderAPI())
		{
		case RendererAPI::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL: return new OpenGLVertexArray();
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}

}