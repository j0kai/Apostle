#include "APpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace Apostle {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		AP_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AP_CORE_ASSERT(status, "Failed to initialise Glad!");

		AP_CORE_INFO("OpenGL Info:");
		printf("  Vendor:   %s\n", glGetString(GL_VENDOR));
		printf("  Renderer: %s\n", glGetString(GL_RENDERER));
		printf("  Version:  %s\n", glGetString(GL_VERSION));

	#ifdef AP_ENABLE_ASSERTS
			int versionMajor, versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

			AP_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Apostle requires at least OpenGL version 4.5 or higher!");
	#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}