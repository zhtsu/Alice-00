#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Renderer/Texture.hpp"

namespace Alice
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D();

    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }

    void Bind() const override;

private:
    std::string m_path;
    uint32_t m_width, m_height;
    uint32_t m_renderer_id;
};

} // namespace Alice
