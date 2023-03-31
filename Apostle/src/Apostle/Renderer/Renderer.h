#pragma once

#include "RenderCommand.h"

#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

#include "Shader.h"

namespace Apostle {

	class Renderer
	{
	public:
		static void BeginScene(PerspectiveCamera& camera);
		static void BeginScene(OrthographicCamera& camera);

		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetRenderAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}