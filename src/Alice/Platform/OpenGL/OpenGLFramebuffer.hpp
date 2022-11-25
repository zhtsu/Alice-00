#pragma once

#include "Alice/Renderer/Framebuffer.hpp"

namespace Alice
{

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    virtual ~OpenGLFramebuffer();

    const FramebufferSpecification& GetSpecification() const override { return m_specification; }

    void Bind() override;
    void Unbind() override;

    uint32_t GetColorAttachmentRendererID() const override { return m_color_attachment; }

    void Invalidate();

private:
    uint32_t m_renderer_id;
    uint32_t m_color_attachment, m_depth_attachment;
    FramebufferSpecification m_specification;
};

} // namespace Alice
