#include "Alice/PCH.hpp"
#include "OpenGLContext.hpp"
#include "Alice/Log/Log.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Alice
{

OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
    : m_window_handle(window_handle)
{
    ALICE_ASSERT(window_handle, "OpenGLContext::OpenGLContext: Window handle is null!");
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_window_handle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ALICE_ASSERT(status, "OpenGLContext::Init: Faied to initialize glad!");
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_window_handle);
}

} // namespace Alice

