#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Renderer/Texture.hpp"
#include "glad/glad.h"

namespace Alice
{

//
// OpenGL 2D 纹理
//
class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D();

    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }

    void SetData(void* data, uint32_t size) override;

    void Bind(uint32_t slot = 0) const override;
    void Unbind() override;

    virtual bool operator==(const Texture& other) const override
    {
        return m_renderer_id == ((OpenGLTexture2D&)other).m_renderer_id;
    }

private:
    std::string m_path;
    uint32_t m_width, m_height;
    uint32_t m_renderer_id;
    GLenum m_internal_formal, m_data_format;
};

} // namespace Alice
