#include "Renderer2D.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Alice/Common/PathHelper.hpp"
#include "Alice/Renderer/RenderCommand.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Alice
{

struct Renderer2DStorage
{
    Ref<VertexArray> quad_vertex_array;
    Ref<Shader> flat_color_shader;
    Ref<Shader> texture_shader;
};

static Renderer2DStorage* s_data;

void Renderer2D::Init()
{
    s_data = new Renderer2DStorage();
    s_data->quad_vertex_array = VertexArray::Create();

    float vertices[4 * 5] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(vertices, sizeof(vertices));
    vertex_buffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" }
    });
    s_data->quad_vertex_array->AddVertexBuffer(vertex_buffer);

    uint32_t indices[6] = { 0, 1, 3, 1, 2, 3 };
    Ref<IndexBuffer> index_buffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    s_data->quad_vertex_array->SetIndexBuffer(index_buffer);

    // Flat color shader
    std::string flat_color_shader_path = 
        PathHelper::GeneratePath(FileType::Shader, "FlatColor.glsl");
    s_data->flat_color_shader = Shader::Create(flat_color_shader_path);
    // Texture shader
    std::string texture_shader_path = 
        PathHelper::GeneratePath(FileType::Shader, "Texture.glsl");
    s_data->texture_shader = Shader::Create(texture_shader_path);
    s_data->texture_shader->Bind();
    s_data->texture_shader->SetInt("u_Texture", 0);
}

void Renderer2D::Shutdown()
{
    delete s_data; 
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    s_data->flat_color_shader->Bind();
    s_data->flat_color_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    s_data->texture_shader->Bind();
    s_data->texture_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
}

void Renderer2D::EndScene()
{

}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    s_data->flat_color_shader->Bind();
    s_data->flat_color_shader->SetFloat4("u_Color", color);

    glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * 
        glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
    s_data->flat_color_shader->SetMat4("u_Transform", transform);

    s_data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_data->quad_vertex_array);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
{
    DrawQuad({ position.x, position.y, 0.0f }, size, texture);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
{
    s_data->texture_shader->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * 
        glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
    s_data->texture_shader->SetMat4("u_Transform", transform);

    texture->Bind();

    s_data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_data->quad_vertex_array);
}

} // namespace Alice
