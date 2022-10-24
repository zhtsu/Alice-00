#include "WindowsWindow.hpp"
#include "Alice/Log/Log.hpp"

namespace Alice
{

static bool s_GLFW_initialized = false;

Window* Window::Create(const WindowProps& props)
{
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
    Init(props);
}

WindowsWindow::~WindowsWindow()
{
    Shutdown();
}

void WindowsWindow::Init(const WindowProps& props)
{
    m_data.title = props.title;
    m_data.width = props.width;
    m_data.height = props.height;

    ALICE_INFO(
        "Create Window for Windows: Title: {}, Size: ({}, {})",
        props.title, props.width, props.height
    );

    if (!s_GLFW_initialized)
    {
        int success = glfwInit();
        ALICE_ASSERT(success, "Could not intialize GLFW!");
    }

    s_GLFW_initialized = true;

    m_window = glfwCreateWindow(
        (int)props.width, (int)props.height,
        props.title.c_str(), nullptr, nullptr
    );
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, &m_data);
    SetVSync(true);
}

void WindowsWindow::Shutdown()
{
    glfwDestroyWindow(m_window);
}

void WindowsWindow::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void WindowsWindow::SetVSync(bool enabled)
{
    if (enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    m_data.Vsync = enabled;
}

bool WindowsWindow::IsVSync() const
{
    return m_data.Vsync;
}

} // namespace Alice
