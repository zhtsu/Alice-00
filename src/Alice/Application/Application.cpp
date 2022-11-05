#include "Application.hpp"
#include "Alice/Log/Log.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Alice/Input/Input.hpp"
#include "Alice/Platform/OpenGL/OpenGLBuffer.hpp"

namespace Alice
{

Application* Application::s_instance = nullptr;

Application::Application() : m_running(true)
{
    ALICE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;
    
    Alice::Log::Init();

    m_window = std::unique_ptr<Window>(Window::Create());
    m_window->SetEventCallback(ALICE_BIND_EVENT_FN(Application::OnEvent));

    m_imgui_layer = std::make_unique<ImGuiLayer>();
    PushOverlay(m_imgui_layer.get());

    glGenVertexArrays(1, &m_vertex_array);
    glBindVertexArray(m_vertex_array);

    float vertices[3 * 3] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    m_vertex_buffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    unsigned int indices[3] = { 0, 1, 2 };
    m_index_buffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

    std::string vertex_src = R"(
        #version 330 core

        layout(location = 0) in vec3 Position;
        out vec3 v_Position;

        void main()
        {
            v_Position = Position;
            gl_Position = vec4(Position, 1.0);
        }
    )";

    std::string fragment_src = R"(
        #version 330 core

        layout(location = 0) out vec4 Color;
        in vec3 v_Position;

        void main()
        {
            Color = vec4(v_Position, 1.0);
        }
    )";

    m_shader.reset(new Shader(vertex_src, fragment_src));
}

Application::~Application()
{

}

void Application::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(ALICE_BIND_EVENT_FN(Application::OnWindowClose));

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
        glClearColor(0.12f, 0.12f, 0.12f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        for (Layer* layer : m_layer_stack)
        {
            layer->OnUpdate();
        }

        m_shader->Bind();
        glBindVertexArray(m_vertex_array);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

bool Application::OnWindowClose(WindowCloseEvent& e)
{
    m_running = false;
    
    return true;
}

} // namespace Alice
