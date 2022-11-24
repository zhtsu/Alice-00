#include "SubTexture2D.hpp"

namespace Alice
{

SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
    : m_texture(texture)
{
    m_texture_coords[0] = { min.x, min.y };
    m_texture_coords[1] = { max.x, min.y };
    m_texture_coords[2] = { max.x, max.y };
    m_texture_coords[3] = { min.x, max.y };
}

Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& size)
{
    glm::vec2 min = {
        (coord.x * size.x) / texture->GetWidth(),
        (coord.y * size.y) / texture->GetHeight()
    };
    glm::vec2 max = {
        ((coord.x + 1) * size.x) / texture->GetWidth(),
        ((coord.y + 1) * size.y) / texture->GetHeight()
    };

    return CreateRef<SubTexture2D>(texture, min, max);
}

} // namespace Alice
