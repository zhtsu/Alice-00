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
    Ref<Shader> texture_shader;
    Ref<Texture2D> white_texture;
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

    s_data->white_texture = Texture2D::Create(1, 1);
    uint32_t white_texture_data = 0xffffffff;
    s_data->white_texture->SetData(&white_texture_data, sizeof(uint32_t));

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
    s_data->texture_shader->SetFloat4("u_Color", color);
    s_data->texture_shader->SetFloat("u_TilingFactor", 1.0f);
    s_data->white_texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * 
        glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
    s_data->texture_shader->SetMat4("u_Transform", transform);

    s_data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_data->quad_vertex_array);
    s_data->white_texture->Unbind();
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
{
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling_factor, tint_color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
{
    s_data->texture_shader->SetFloat4("u_Color", tint_color);
    s_data->texture_shader->SetFloat("u_TilingFactor", tiling_factor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * 
        glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
    s_data->texture_shader->SetMat4("u_Transform", transform);

    s_data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_data->quad_vertex_array);
    texture->Unbind();
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
    s_data->texture_shader->SetFloat4("u_Color", color);
    s_data->texture_shader->SetFloat("u_TilingFactor", 1.0f);
    s_data->white_texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
    s_data->texture_shader->SetMat4("u_Transform", transform);

    s_data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_data->quad_vertex_array);
    s_data->white_texture->Unbind();
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
{
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling_factor, tint_color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& tint_color)
{
    s_data->texture_shader->SetFloat4("u_Color", tint_color);
    s_data->texture_shader->SetFloat("u_TilingFactor", tiling_factor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0), position)
        * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
    s_data->texture_shader->SetMat4("u_Transform", transform);

    s_data->quad_vertex_array->Bind();
    RenderCommand::DrawIndexed(s_data->quad_vertex_array);
    texture->Unbind();
}

} // namespace Alice
