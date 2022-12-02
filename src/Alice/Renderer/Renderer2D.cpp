#include "Renderer2D.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Alice/Common/PathHelper.hpp"
#include "Alice/Renderer/RenderCommand.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Alice
{

struct QuadVertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texture_coord;
    float texture_index;
    float tiling_factor;
};

struct Renderer2DData
{
    static const uint32_t kMaxQuads = 20000;
    static const uint32_t kMaxVertices = kMaxQuads * 4;
    static const uint32_t kMaxIndices = kMaxQuads * 6;
    static const uint32_t kMaxTextureSlots = 32;

    Ref<VertexArray> quad_vertex_array;
    Ref<VertexBuffer> quad_vertex_buffer;
    Ref<Shader> texture_shader;
    Ref<Texture2D> white_texture;

    uint32_t quad_index_count = 0;
    QuadVertex* quad_vertex_buffer_base = nullptr;
    QuadVertex* quad_vertex_buffer_ptr = nullptr;

    std::array<Ref<Texture2D>, kMaxTextureSlots> texture_slots;
    uint32_t Texture_slot_index = 1; // 0 slot use pure white texture

    glm::vec4 quad_vertex_positions[4];

    Renderer2D::Statistics stats;
};

static Renderer2DData s_data;

void Renderer2D::Init()
{
    s_data.quad_vertex_array = VertexArray::Create();

    s_data.quad_vertex_buffer = VertexBuffer::Create(s_data.kMaxVertices * sizeof(QuadVertex));
    s_data.quad_vertex_buffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" },
        { ShaderDataType::Float2, "a_TexCoord" },
        { ShaderDataType::Float, "a_TexIndex" },
        { ShaderDataType::Float, "a_TilingFactor" }
    });
    s_data.quad_vertex_array->AddVertexBuffer(s_data.quad_vertex_buffer);

    s_data.quad_vertex_buffer_base = new QuadVertex[s_data.kMaxVertices];

    uint32_t* indices = new uint32_t[s_data.kMaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.kMaxIndices; i += 6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;

        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }

    Ref<IndexBuffer> quad_index_buffer = IndexBuffer::Create(indices, s_data.kMaxIndices);
    s_data.quad_vertex_array->SetIndexBuffer(quad_index_buffer);
    delete[] indices;

    s_data.white_texture = Texture2D::Create(1, 1);
    uint32_t white_texture_data = 0xffffffff;
    s_data.white_texture->SetData(&white_texture_data, sizeof(uint32_t));

    int32_t samplers[s_data.kMaxTextureSlots];
    for (int i = 0; i < s_data.kMaxTextureSlots; i++)
    {
        samplers[i] = i;
    }

    // Texture shader
    std::string texture_shader_path = 
        PathHelper::GeneratePath(FileType::Shader, "Texture.glsl");
    s_data.texture_shader = Shader::Create(texture_shader_path);
    s_data.texture_shader->Bind();
    s_data.texture_shader->SetIntArray("u_Textures", samplers, s_data.kMaxTextureSlots);

    s_data.texture_slots[0] = s_data.white_texture;

    s_data.quad_vertex_positions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_data.quad_vertex_positions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_data.quad_vertex_positions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    s_data.quad_vertex_positions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}

void Renderer2D::Shutdown()
{
    
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    s_data.texture_shader->Bind();
    s_data.texture_shader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;

    s_data.Texture_slot_index = 1;
}

void Renderer2D::EndScene()
{
    uint32_t data_size = (uint8_t*)s_data.quad_vertex_buffer_ptr - (uint8_t*)s_data.quad_vertex_buffer_base;
    s_data.quad_vertex_buffer->SetData(s_data.quad_vertex_buffer_base, data_size);

    Flush();
}

void Renderer2D::Flush()
{
    for (int i = 0; i < s_data.Texture_slot_index; i++)
    {
        s_data.texture_slots[i]->Bind(i);
    }

    RenderCommand::DrawIndexed(s_data.quad_vertex_array, s_data.quad_index_count);
    s_data.stats.draw_calls++;
}

void Renderer2D::FlushAndReset()
{
    EndScene();

    s_data.quad_index_count = 0;
    s_data.quad_vertex_buffer_ptr = s_data.quad_vertex_buffer_base;

    s_data.Texture_slot_index = 1;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    DrawQuad({ position.x, position.y, 0.0f }, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
    
    DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& color)
{
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling_factor, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& color)
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    DrawQuad(transform, texture, tiling_factor, color);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tiling_factor, const glm::vec4& color)
{
    DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tiling_factor, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tiling_factor, const glm::vec4& color)
{
    constexpr int quad_vertex_count = 4;
    const glm::vec2* texture_coords = subtexture->GetTextureCoords();
    const Ref<Texture2D> texture = subtexture->GetTexture();

    if (s_data.quad_index_count >= Renderer2DData::kMaxIndices)
    {
        FlushAndReset();
    }

    float texture_index = 0.0f;
    for (int i = 1; i < s_data.Texture_slot_index; i++)
    {
        if (*s_data.texture_slots[i].get() == *texture.get())
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        texture_index = (float)s_data.Texture_slot_index;
        s_data.texture_slots[s_data.Texture_slot_index] = texture;
        s_data.Texture_slot_index++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (int i = 0 ; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->texture_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;

    s_data.stats.quad_count++;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
{
    constexpr int quad_vertex_count = 4;
    constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    constexpr float texture_index = 0.0f;
    constexpr float tiling_factor = 1.0f;

    if (s_data.quad_index_count >= Renderer2DData::kMaxIndices)
    {
        FlushAndReset();
    }

    for (int i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->texture_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;

    s_data.stats.quad_count++;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& color)
{
    constexpr int quad_vertex_count = 4;
    constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (s_data.quad_index_count >= Renderer2DData::kMaxIndices)
    {
        FlushAndReset();
    }

    float texture_index = 0.0f;
    for (int i = 1; i < s_data.Texture_slot_index; i++)
    {
        if (*s_data.texture_slots[i].get() == *texture.get())
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        texture_index = (float)s_data.Texture_slot_index;
        s_data.texture_slots[s_data.Texture_slot_index] = texture;
        s_data.Texture_slot_index++;
    }

    for (int i = 0 ; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->texture_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;

    s_data.stats.quad_count++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
{
    constexpr int quad_vertex_count = 4;
    constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
    constexpr float texture_index = 0.0f;
    constexpr float tiling_factor = 1.0f;

    if (s_data.quad_index_count >= Renderer2DData::kMaxIndices)
    {
        FlushAndReset();
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (int i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->texture_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;

    s_data.stats.quad_count++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& color)
{
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling_factor, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor, const glm::vec4& color)
{
    constexpr int quad_vertex_count = 4;
    constexpr glm::vec2 texture_coords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

    if (s_data.quad_index_count >= Renderer2DData::kMaxIndices)
    {
        FlushAndReset();
    }

    float texture_index = 0.0f;
    for (int i = 1; i < s_data.Texture_slot_index; i++)
    {
        if (*s_data.texture_slots[i].get() == *texture.get())
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        texture_index = (float)s_data.Texture_slot_index;
        s_data.texture_slots[s_data.Texture_slot_index] = texture;
        s_data.Texture_slot_index++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (int i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->texture_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;

    s_data.stats.quad_count++;
}

void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tiling_factor, const glm::vec4& color)
{
    DrawRotatedQuad(position, size, rotation, subtexture, tiling_factor, color);
}

void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tiling_factor, const glm::vec4& color)
{
    constexpr int quad_vertex_count = 4;
    const glm::vec2* texture_coords = subtexture->GetTextureCoords();
    const Ref<Texture2D> texture = subtexture->GetTexture();

    if (s_data.quad_index_count >= Renderer2DData::kMaxIndices)
    {
        FlushAndReset();
    }

    float texture_index = 0.0f;
    for (int i = 1; i < s_data.Texture_slot_index; i++)
    {
        if (*s_data.texture_slots[i].get() == *texture.get())
        {
            texture_index = (float)i;
            break;
        }
    }

    if (texture_index == 0.0f)
    {
        texture_index = (float)s_data.Texture_slot_index;
        s_data.texture_slots[s_data.Texture_slot_index] = texture;
        s_data.Texture_slot_index++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
        * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

    for (int i = 0; i < quad_vertex_count; i++)
    {
        s_data.quad_vertex_buffer_ptr->position = transform * s_data.quad_vertex_positions[i];
        s_data.quad_vertex_buffer_ptr->color = color;
        s_data.quad_vertex_buffer_ptr->texture_coord = texture_coords[i];
        s_data.quad_vertex_buffer_ptr->texture_index = texture_index;
        s_data.quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
        s_data.quad_vertex_buffer_ptr++;
    }

    s_data.quad_index_count += 6;

    s_data.stats.quad_count++;
}

void Renderer2D::ResetStats()
{
    memset(&s_data.stats, 0, sizeof(Renderer2D::Statistics));
}

Renderer2D::Statistics Renderer2D::GetStats()
{
    return s_data.stats;
}

} // namespace Alice
