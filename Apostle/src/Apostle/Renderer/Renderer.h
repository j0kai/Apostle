#pragma once

#include "RenderCommand.h"

namespace Apostle {

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetRenderAPI() { return RendererAPI::GetAPI(); }
	};
}