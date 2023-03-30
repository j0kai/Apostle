#include "APpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Apostle {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}