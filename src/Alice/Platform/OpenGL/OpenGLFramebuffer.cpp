#include "OpenGLFramebuffer.hpp"
#include "Alice/Log/Log.hpp"
#include <glad/glad.h>

namespace Alice
{

static const uint32_t s_max_framebuffer_size = 8192;

namespace Utils
{
    static GLenum TextureTarget(bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTextures(bool multisampled, uint32_t* out_id, uint32_t count)
    {
        glCreateTextures(TextureTarget(multisampled), count, out_id);
    }

    static void BindTexture(bool multisampled, uint32_t id)
    {
        glBindTexture(TextureTarget(multisampled), id);
    }

    static void AttachColorTexture(uint32_t id, int samples, GLenum internal_format, GLenum format, uint32_t width, uint32_t height, int index)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format, width, height, GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
    }

    static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachment_type, uint32_t width, uint32_t height)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, TextureTarget(multisampled), id, 0);
    }

    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::DEPTH24_STENCIL8:
                return true;
        }

        return false;
    }

    static GLenum ToGLFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::RGBA8:
                return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER:
                return GL_RED_INTEGER;
        }

        ALICE_ASSERT(false, "Format invalid!");
        return 0;
    }
}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    : m_specification(spec)
{
    for (auto& spec : m_specification.attachments.attachments)
    {
        if (!Utils::IsDepthFormat(spec.texture_format))
            m_color_attachment_specs.emplace_back(spec);
        else
            m_depth_attachment_spec = spec;
    }

    Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &m_renderer_id);
    glDeleteTextures(m_color_attachments.size(), m_color_attachments.data());
    glDeleteTextures(1, &m_depth_attachment);
}

uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const
{
    ALICE_ASSERT(index < m_color_attachments.size(), "Index is invalid!");
    return m_color_attachments[index];
}

void OpenGLFramebuffer::Invalidate()
{
    if (m_renderer_id)
    {
        glDeleteFramebuffers(1, &m_renderer_id);
        glDeleteTextures(m_color_attachments.size(), m_color_attachments.data());
        glDeleteTextures(1, &m_depth_attachment);

        m_color_attachments.clear();
        m_depth_attachment = 0;
    }

    glCreateFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    bool multisampled = m_specification.samples > 1;

    // Attachments
    if (m_color_attachment_specs.size())
    {
        m_color_attachments.resize(m_color_attachment_specs.size());
        Utils::CreateTextures(multisampled, m_color_attachments.data(), m_color_attachments.size());

        for (int i = 0; i < m_color_attachments.size(); i++)
        {
            Utils::BindTexture(multisampled, m_color_attachments[i]);
            switch (m_color_attachment_specs[i].texture_format)
            {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_color_attachments[i], m_specification.samples,
                        GL_RGBA8, GL_RGBA, m_specification.width, m_specification.height, i);
                    break;
                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_color_attachments[i], m_specification.samples,
                        GL_R32I, GL_RED_INTEGER, m_specification.width, m_specification.height, i);
                    break;
            }
        }
    }

    if (m_depth_attachment_spec.texture_format != FramebufferTextureFormat::None)
    {
        Utils::CreateTextures(multisampled, &m_depth_attachment, 1);
        Utils::BindTexture(multisampled, m_depth_attachment);
        switch (m_depth_attachment_spec.texture_format)
        {
            case FramebufferTextureFormat::DEPTH24_STENCIL8:
                Utils::AttachDepthTexture(m_depth_attachment, m_specification.samples,
                    GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
                break;
        }
    }

    if (m_color_attachments.size() > 1)
    {
        ALICE_ASSERT(m_color_attachments.size() <= 4, "Color attachments's count is more than 4!");
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_color_attachments.size(), buffers);
    }
    else if (m_color_attachments.empty())
    {
        glDrawBuffer(GL_NONE);
    }

    ALICE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
    glViewport(0, 0, m_specification.width, m_specification.height);
}

void OpenGLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > s_max_framebuffer_size || height > s_max_framebuffer_size)
    {
        ALICE_WARN("Attempted to resize framebuffer to ({}, {})", width, height);
        return;
    }

    m_specification.width = width;
    m_specification.height = height;

    Invalidate();
}

int OpenGLFramebuffer::ReadPixel(uint32_t attachment_index, int x, int y) const
{
    ALICE_ASSERT(attachment_index < m_color_attachments.size(), "Index is invalid!");
    
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
    int pixel_data = -1;
    // @TODO:
    // !!! BUG !!!
    // The glReadPixels return a incorrect value when mouse hover a entity
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
    return pixel_data;
}

void OpenGLFramebuffer::ClearAttachment(uint32_t attachment_index, int value)
{
    ALICE_ASSERT(attachment_index < m_color_attachments.size(), "Index is invalid!");

    auto& spec = m_color_attachment_specs[attachment_index];
    glClearTexImage(m_color_attachments[attachment_index], 0, 
        Utils::ToGLFormat(spec.texture_format), GL_INT, &value);
}

} // namespace Alice
