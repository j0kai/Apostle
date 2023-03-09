#pragma once
#include "Apostle/Renderer/RenderContext.h"

struct GLFWwindow;

namespace Apostle {


	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};

}