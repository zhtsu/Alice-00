#include "OpenGLFramebuffer.hpp"
#include "Alice/Log/Log.hpp"
#include <glad/glad.h>

namespace Alice
{

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
{
    Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &m_renderer_id);
}

void OpenGLFramebuffer::Invalidate()
{
    glCreateFramebuffers(1, &m_renderer_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment);
    glBindTexture(GL_TEXTURE_2D, m_color_attachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment, 0);

    //
    glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment);
    glBindTexture(GL_TEXTURE_2D, m_depth_attachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment, 0);

    ALICE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
}

void OpenGLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace Alice
