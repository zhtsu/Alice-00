#pragma once

#include "Alice/Renderer/GraphicsContext.hpp"

struct GLFWwindow;

namespace Alice
{

//
// OpenGL 渲染上下文
//
class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* window_handle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_window_handle;
};

} // namespace Alice
