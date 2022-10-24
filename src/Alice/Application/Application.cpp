#include "Application.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Log/Log.hpp"
#include "GLFW/glfw3.h"

namespace Alice
{

Application::Application() : m_running(true)
{
    Alice::Log::Init();
    m_window = std::unique_ptr<Window>(Window::Create());
}

Application::~Application()
{

}


void Application::Run()
{
    while(m_running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_window->OnUpdate();
    }
}

} // namespace Alice
