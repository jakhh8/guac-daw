#include "gmpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Guacamole {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		GM_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
        GM_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GM_CORE_ASSERT(status, "Failed to initialize Glad!");

		GM_CORE_INFO("OpenGL Info:");
		GM_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		GM_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GM_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		GM_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Guacamole requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
        GM_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
	
}