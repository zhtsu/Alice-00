#include "Alice/Alice.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class ExampleLayer : public Alice::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_camera(-1.6, 1.6, -0.9, 0.9)
    {
        m_vertex_array.reset(Alice::VertexArray::Create());

        float vertices[4 * 5] = {
            0.7f,  0.7f, 0.0f, 1.0f, 1.0f,   // 右上
            0.7f, -0.7f, 0.0f, 1.0f, 0.0f,   // 右下
            -0.7f, -0.7f, 0.0f, 0.0f, 0.0f,  // 左下
            -0.7f,  0.7f, 0.0f, 0.0f, 1.0f   // 左上
        };

        Alice::Ref<Alice::VertexBuffer> vertex_buffer;
        vertex_buffer.reset(Alice::VertexBuffer::Create(vertices, sizeof(vertices)));
        vertex_buffer->SetLayout({
            { Alice::ShaderDataType::Float3, "a_Position" },
            { Alice::ShaderDataType::Float2, "a_TexCoord" }
        });
        m_vertex_array->AddVertexBuffer(vertex_buffer);

        uint32_t indices[6] = { 0, 1, 3, 1, 2, 3 };
        Alice::Ref<Alice::IndexBuffer> index_buffer;
        index_buffer.reset(Alice::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_vertex_array->SetIndexBuffer(index_buffer);

        std::string vertex_src = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 0) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;

            void main()
            {
                v_TexCoord = a_TexCoord;
                gl_Position = u_ViewProjection *  u_Transform * vec4(a_Position, 1.0);
            }
        )";
        std::string fragment_src = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform sampler2D u_Texture;

            void main()
            {
                color = texture(u_Texture, v_TexCoord - 0.5);
            }
        )";
        m_shader.reset(Alice::Shader::Create(vertex_src, fragment_src));

        std::string img_path = Alice::PathHelper::GeneratePath(
            Alice::FileType::Image,
            "KFC.jpg"
        );

        m_texture = Alice::Texture2D::Create(img_path);

        std::dynamic_pointer_cast<Alice::OpenGLShader>(m_shader)
            ->Bind();
        std::dynamic_pointer_cast<Alice::OpenGLShader>(m_shader)
            ->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Alice::Timestep ts) override
    {
        if (Alice::Input::IsKeyPressed(ALICE_KEY_LEFT))
            m_camera_position.x -= m_camera_move_speed * ts;
        else if (Alice::Input::IsKeyPressed(ALICE_KEY_RIGHT))
            m_camera_position.x += m_camera_move_speed * ts;

        if (Alice::Input::IsKeyPressed(ALICE_KEY_UP))
            m_camera_position.y += m_camera_move_speed * ts;
        else if (Alice::Input::IsKeyPressed(ALICE_KEY_DOWN))
            m_camera_position.y -= m_camera_move_speed * ts;

        if (Alice::Input::IsKeyPressed(ALICE_KEY_A))
            m_camera_rotation += m_camera_rotation_speed * ts;
        else if (Alice::Input::IsKeyPressed(ALICE_KEY_D))
            m_camera_rotation -= m_camera_rotation_speed * ts;

        Alice::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Alice::RenderCommand::Clear();

        m_camera.SetPosition(m_camera_position);
        m_camera.SetRotation(m_camera_rotation);

        Alice::Renderer::BeginScene(m_camera);

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
        Alice::Renderer::Submit(m_vertex_array, m_shader);

        Alice::Renderer::EndScene();
    }

    void OnEvent(Alice::Event& event) override
    {
        
    }

private:
    Alice::Ref<Alice::Shader> m_shader;
    Alice::Ref<Alice::VertexArray> m_vertex_array;

    Alice::Ref<Alice::Texture2D> m_texture;

    Alice::OrthographicCamera m_camera;
    glm::vec3 m_camera_position = { 0.0f, 0.0f, 0.0f };
    float m_camera_move_speed = 1.0f;

    float m_camera_rotation = 0.0f;
    float m_camera_rotation_speed = 36.0f;

    glm::vec3 m_color = { 0.2f, 0.3f, 0.8f };
};

int main(int argc, char* argv[])
{
    auto app = new Alice::Application();
    app->PushLayer(new ExampleLayer());
    app->Run();
    delete app;

    return 0;
}
