#include "Sandbox2D.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
    :Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
    m_square_vao = Alice::VertexArray::Create();

    float vertices[3 * 4] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    Alice::Ref<Alice::VertexBuffer> vertex_buffer = Alice::VertexBuffer::Create(vertices, sizeof(vertices));
    vertex_buffer->SetLayout({
        { Alice::ShaderDataType::Float3, "a_Position" }
    });
    m_square_vao->AddVertexBuffer(vertex_buffer);

    uint32_t indices[6] = { 0, 1, 3, 1, 2, 3 };
    Alice::Ref<Alice::IndexBuffer> index_buffer = Alice::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_square_vao->SetIndexBuffer(index_buffer);

    std::string flat_color_shader_path = Alice::PathHelper::GeneratePath(
        Alice::FileType::Shader,
        "FlatColor.glsl"
    );

    m_flat_color_shader = Alice::Shader::Create(flat_color_shader_path);

    
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Alice::Timestep ts)
{
    m_camera_controller.OnUpdate(ts);

    Alice::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Alice::RenderCommand::Clear();

    Alice::Renderer::BeginScene(m_camera_controller.GetCamera());

    std::dynamic_pointer_cast<Alice::OpenGLShader>(m_flat_color_shader)->Bind();
    std::dynamic_pointer_cast<Alice::OpenGLShader>(m_flat_color_shader)
        ->UploadUniformFloat4("u_Color", m_square_color);

    m_flat_color_shader->Bind();
    Alice::Renderer::Submit(m_square_vao, m_flat_color_shader);

    Alice::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_square_color));
    ImGui::End();
}

void Sandbox2D::OnEvent(Alice::Event& event)
{
    m_camera_controller.OnEvent(event);
}
