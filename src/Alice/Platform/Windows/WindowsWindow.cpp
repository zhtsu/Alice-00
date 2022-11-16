#include "WindowsWindow.hpp"
#include "Alice/Log/Log.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Events/KeyEvent.hpp"
#include "Alice/Events/MouseEvent.hpp"
#include "glad/glad.h"
#include "Alice/Platform/OpenGL/OpenGLContext.hpp"

namespace Alice
{

static bool s_GLFW_initialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
    ALICE_ERROR("GLFW Error {}: {}", error, description);
}

Window* Window::Create(const WindowProps& props)
{
    return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
    : m_window(nullptr), m_context(nullptr)
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
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    s_GLFW_initialized = true;

    m_window = glfwCreateWindow(
        (int)props.width, (int)props.height,
        props.title.c_str(), nullptr, nullptr
    );
    
    m_context = new OpenGLContext(m_window);
    m_context->Init();

    glfwSetWindowUserPointer(m_window, &m_data);
    SetVSync(true);

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        
        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        data.event_callback(event);
    });
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        
        WindowCloseEvent event;
        data.event_callback(event);
    });
    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(keycode);
        data.event_callback(event);
    });
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(key, 0);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.event_callback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.event_callback(event);
                break;
            }
        }
    });
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    
        switch (action)
        {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.event_callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.event_callback(event);
                break;
            }
        }
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x_offset, double y_offset){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)x_offset, (float)y_offset);
        data.event_callback(event);
    });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x_pos, double y_pos){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)x_pos, (float)y_pos);
        data.event_callback(event);
    });
}

void WindowsWindow::Shutdown()
{
    if (m_window != nullptr)
        glfwDestroyWindow(m_window);
}

void WindowsWindow::OnUpdate()
{
    glfwPollEvents();
    m_context->SwapBuffers();
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
