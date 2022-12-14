#include "OpenGLTexture.hpp"
#include "Alice/Log/Log.hpp"
#include "stb_image.h"


namespace Alice
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    m_internal_formal = GL_RGBA8;
    m_data_format = GL_RGBA;

    glGenTextures(1, &m_renderer_id);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_path(path)
{
    int width, height, channels;
    
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    ALICE_ASSERT(data, "OpenGLTexture2D::OpenGLTexture2D: Failed to load image!");

    m_width = width;
    m_height = height;

    m_internal_formal = 0;
    m_data_format = 0;

    if (channels == 4)
    {
        m_internal_formal = GL_RGBA8;
        m_data_format = GL_RGBA;
    }
    else if (channels == 3)
    {
        m_internal_formal = GL_RGB8;
        m_data_format = GL_RGB;
    }

    ALICE_ASSERT(m_internal_formal & m_data_format, "OpenGLTexture2D::OpenGLTexture2D: Format not supported!");

    glGenTextures(1, &m_renderer_id);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, m_data_format, width, height, 0, m_data_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_renderer_id);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size)
{
    uint32_t bpp = m_data_format == GL_RGBA ? 4 : 3;
    ALICE_ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
    glTexImage2D(GL_TEXTURE_2D, 0, m_data_format, m_width, m_height, 0, m_data_format, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void OpenGLTexture2D::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace Alice
