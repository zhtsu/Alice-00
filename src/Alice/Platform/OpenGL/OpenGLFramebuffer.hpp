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

    void Resize(uint32_t width, uint32_t height) override;

    uint32_t GetColorAttachmentRendererID() const override { return m_color_attachment; }

    void Invalidate();

private:
    uint32_t m_renderer_id = 0;
    uint32_t m_color_attachment = 0, m_depth_attachment = 0;
    FramebufferSpecification m_specification;
};

} // namespace Alice
