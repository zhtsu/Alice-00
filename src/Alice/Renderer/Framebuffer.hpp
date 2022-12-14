#pragma once

#include "Alice/Core/Base.hpp"

namespace Alice
{

enum class FramebufferTextureFormat
{
    None = 0,
    // Color
    RGB8,
    RED_INTEGER,
    // Depth/Stencil
    DEPTH24_STENCIL8,
    // Defaults
    Depth = DEPTH24_STENCIL8
};

struct FramebufferTextureSpecification
{
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
        : texture_format(format) {}

    FramebufferTextureFormat texture_format = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> _attachments)
        : attachments(_attachments) {}

    std::vector<FramebufferTextureSpecification> attachments;
};

struct FramebufferSpecification
{
    FramebufferAttachmentSpecification attachments;

    uint32_t width = 0, height = 0;
    uint32_t samples = 1;

    bool swap_chain_target = false;
};

class Framebuffer
{
public:
    virtual const FramebufferSpecification& GetSpecification() const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual int ReadPixel(uint32_t attachment_index, int x, int y) const = 0;

    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};

} // namespace Alice
