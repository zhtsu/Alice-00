#include "Sandbox/Sandbox2D.hpp"

class ExampleLayer : public Alice::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_camera_controller(1280.0f / 720.0f)
    {
        m_vertex_array = Alice::VertexArray::Create();

        float vertices[4 * 5] = {
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // 左下
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // 左上
        };

        Alice::Ref<Alice::VertexBuffer> vertex_buffer = Alice::VertexBuffer::Create(vertices, sizeof(vertices));
        vertex_buffer->SetLayout({
            { Alice::ShaderDataType::Float3, "a_Position" },
            { Alice::ShaderDataType::Float2, "a_TexCoord" }
        });
        m_vertex_array->AddVertexBuffer(vertex_buffer);

        uint32_t indices[6] = { 0, 1, 3, 1, 2, 3 };
        Alice::Ref<Alice::IndexBuffer> index_buffer = Alice::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_vertex_array->SetIndexBuffer(index_buffer);

        std::string texture_shader_path = Alice::PathHelper::GeneratePath(
            Alice::FileType::Shader,
            "Texture.glsl"
        );

        auto texture_shader = m_shader_library.Load("Texture", texture_shader_path);

        std::string img_path = Alice::PathHelper::GeneratePath(
            Alice::FileType::Image,
            "KFC.jpg"
        );

        m_texture = Alice::Texture2D::Create(img_path);

        std::dynamic_pointer_cast<Alice::OpenGLShader>(texture_shader)
            ->Bind();
        std::dynamic_pointer_cast<Alice::OpenGLShader>(texture_shader)
            ->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Alice::Timestep ts) override
    {
        m_camera_controller.OnUpdate(ts);

        Alice::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Alice::RenderCommand::Clear();

        Alice::Renderer::BeginScene(m_camera_controller.GetCamera());

        // glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        // for (int y = 0; y < 20; y++)
        // {
        //     for (int x = 0; x < 20; x++)
        //     {
        //         glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
        //         glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
        //     }
        // }

        m_texture->Bind();
        Alice::Renderer::Submit(m_vertex_array, m_shader_library.Get("Texture"));

        Alice::Renderer::EndScene();
    }

    void OnEvent(Alice::Event& event) override
    {
        m_camera_controller.OnEvent(event);
    }

private:
    Alice::ShaderLibrary m_shader_library;
    Alice::Ref<Alice::VertexArray> m_vertex_array;
    Alice::Ref<Alice::Texture2D> m_texture;

    Alice::OrthographicCameraController m_camera_controller;

    glm::vec3 m_color = { 0.2f, 0.3f, 0.8f };
};

int main(int argc, char* argv[])
{
    auto app = new Alice::Application();
    app->PushLayer(new Sandbox2D());
    app->Run();
    delete app;

    return 0;
}
