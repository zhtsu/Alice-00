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

    int ReadPixel(uint32_t attachment_index, int x, int y) const override;

    uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;

    void Invalidate();

private:
    uint32_t m_renderer_id = 0;

    FramebufferSpecification m_specification;
    std::vector<FramebufferTextureSpecification> m_color_attachment_specs;
    FramebufferTextureSpecification m_depth_attachment_spec;

    std::vector<uint32_t> m_color_attachments;
    uint32_t m_depth_attachment = 0;
};

} // namespace Alice
