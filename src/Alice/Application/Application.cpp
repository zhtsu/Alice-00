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

    for (auto it = m_layer_stack.end(); it != m_layer_stack.begin(); )
    {
        (*--it)->OnEvent(event);
        if (event.m_handled)
        {
            break;
        }
    }
}

void Application::Run()
{
    while(m_running)
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Layer* layer : m_layer_stack)
        {
            layer->OnUpdate();
        }

        m_window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer)
{
    m_layer_stack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
    m_layer_stack.PushOverlay(layer);
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_running = false;
    return true;
}

} // namespace Alice
