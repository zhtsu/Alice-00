#pragma once

#include <glm/glm.hpp>
#include "Texture.hpp"

namespace Alice
{

class SubTexture2D
{
public:
    SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

    const Ref<Texture2D> GetTexture() const { return m_texture; }
    const glm::vec2* GetTextureCoords() const { return m_texture_coords; }

    static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coord, const glm::vec2& cell_size, const glm::vec2& sprite_size = { 1.0f, 1.0f });

private:
    Ref<Texture2D> m_texture;
    glm::vec2 m_texture_coords[4];
};



} // namespace Alice
