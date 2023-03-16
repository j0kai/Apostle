#pragma once


namespace Apostle {

	enum class RendererAPI
	{
		None = 0, 
		OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RendererAPI GetRenderAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};
}