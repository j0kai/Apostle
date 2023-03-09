#include "APpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include <glad/glad.h>

namespace Apostle {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AP_CORE_ASSERT(status, "Failed to initialise Glad!");
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}