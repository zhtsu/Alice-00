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
    m_window->SetEventCallback(
        std::bind(&Application::OnEvent, this, std::placeholders::_1)
    );
}

Application::~Application()
{

}

void Application::OnEvent(Alice::Event &event)
{
    ALICE_INFO("{}", event);
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
