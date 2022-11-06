#include "Application.hpp"
#include "Alice/Log/Log.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Alice/Input/Input.hpp"
#include "Alice/Platform/OpenGL/OpenGLBuffer.hpp"
#include "Alice/Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Alice
{

Application* Application::s_instance = nullptr;

Application::Application()
    : m_running(true)
{
    ALICE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;
    
    Alice::Log::Init();

    m_window = std::unique_ptr<Window>(Window::Create());
    m_window->SetEventCallback(ALICE_BIND_EVENT_FN(Application::OnEvent));

    m_imgui_layer = std::make_unique<ImGuiLayer>();
    PushOverlay(m_imgui_layer.get());

    m_vertex_array.reset(VertexArray::Create());

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.8f, 0.2f, 1.0f,
    };
    m_vertex_buffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position"},
            { ShaderDataType::Float4, "a_Color"}
        };

        m_vertex_buffer->SetLayout(layout);
    }
    m_vertex_array->AddVertexBuffer(m_vertex_buffer);

    unsigned int indices[3] = { 0, 1, 2 };
    m_index_buffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
    m_vertex_array->SetIndexBuffer(m_index_buffer);

    m_square_vao.reset(VertexArray::Create());

    float square_vertices[4 * 3] = {
        -0.75f, -0.75f, 0.0f,
        0.75f, -0.75f, 0.0f,
        0.75f, 0.75f, 0.0f,
        -0.75f, 0.75f, 0.0f
    };

    std::shared_ptr<VertexBuffer> square_vbo;
    square_vbo.reset(VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
    {
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position"}
        };

        square_vbo->SetLayout(layout);
    }
    m_square_vao->AddVertexBuffer(square_vbo);

    unsigned int square_indices[6] = { 0, 1, 2, 2, 3, 0 };
    std::shared_ptr<IndexBuffer> square_ebo;
    square_ebo.reset(IndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t)));
    m_square_vao->SetIndexBuffer(square_ebo);

    std::string vertex_src = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        out vec3 v_Position;
        out vec4 v_Color;

        void main()
        {
            v_Position = a_Position;
            v_Color = a_Color;
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string fragment_src = R"(
        #version 330 core

        layout(location = 0) out vec4 Color;

        in vec3 v_Position;
        in vec4 v_Color;

        void main()
        {
            Color = v_Color;
        }
    )";

    std::string vertex_src2 = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;

        void main()
        {
            gl_Position = vec4(a_Position, 1.0);
        }
    )";

    std::string fragment_src2 = R"(
        #version 330 core

        layout(location = 0) out vec4 Color;

        void main()
        {
            Color = vec4(0.2, 0.3, 0.8, 1.0);
        }
    )";
    m_blue_shader.reset(new Shader(vertex_src2, fragment_src2));
    
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

        m_blue_shader->Bind();
        m_square_vao->Bind();
        glDrawElements(GL_TRIANGLES, m_square_vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        m_shader->Bind();
        m_vertex_array->Bind();
        glDrawElements(GL_TRIANGLES, m_index_buffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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
