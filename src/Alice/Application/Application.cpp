#include "Application.hpp"
#include "Alice/Log/Log.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Alice/Core/Timestep.hpp"
#include "Alice/Renderer/Renderer.hpp"

namespace Alice
{

Application* Application::s_instance = nullptr;

Application::Application(const std::string& name)
    : m_imgui_layer(nullptr)
{
    ALICE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;
    
    Alice::Log::Init();

    m_window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
    m_window->SetEventCallback(ALICE_BIND_EVENT_FN(Application::OnEvent));

    Renderer::Init();    

    m_imgui_layer = new ImGuiLayer();
    PushOverlay(m_imgui_layer);
}

Application::~Application()
{

}

void Application::Close()
{
    m_running = false;
}

void Application::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(ALICE_BIND_EVENT_FN(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(ALICE_BIND_EVENT_FN(Application::OnWindowResize));

    // ALICE_TRACE("{}", event);

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
        float time = (float)glfwGetTime();
        Timestep timestep = time - m_last_frame_time;
        m_last_frame_time = time;

        if (!m_minimized)
        {
            for (Layer* layer : m_layer_stack)
            {
                layer->OnUpdate(timestep);
            }
        }

        m_imgui_layer->Begin();
        for (Layer* layer : m_layer_stack)
        {
            layer->OnImGuiRender();
        }
        m_imgui_layer->End();

        // ALICE_TRACE("{}, {}", Input::GetMouseX(), Input::GetMouseY());

        m_window->OnUpdate();
    }
}

void Application::PushLayer(Layer* layer)
{
    m_layer_stack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PushOverlay(Layer* layer)
{
    m_layer_stack.PushOverlay(layer);
    layer->OnAttach();
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
    m_running = false;

    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
    if (event.GetWidth() == 0 || event.GetHeight() == 0)
    {
        m_minimized = true;

        return false;
    }
    
    m_minimized = false;
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

    return true;
}

} // namespace Alice
