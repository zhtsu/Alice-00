#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Renderer/Texture.hpp"

namespace Alice
{

//
// @TODO: 
// 纹理加载后采样器坐标会向右偏移 0.5
// 原因未知
//
class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const std::string& path);
    ~OpenGLTexture2D();

    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }

    void Bind(uint32_t slot = 0) const override;

private:
    std::string m_path;
    uint32_t m_width, m_height;
    uint32_t m_renderer_id;
};

} // namespace Alice
