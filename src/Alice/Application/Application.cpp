#include "Application.hpp"
#include "Alice/Log/Log.hpp"
#include "GLFW/glfw3.h"

namespace Alice
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application() : m_running(true)
{
    Alice::Log::Init();
    m_window = std::unique_ptr<Window>(Window::Create());
    m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));
}

Application::~Application()
{

}

void Application::OnEvent(Alice::Event &event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

    ALICE_TRACE("{}", event);
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

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_running = false;
    return true;
}

} // namespace Alice
