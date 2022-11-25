#pragma once

#include "Alice/Core/Base.hpp"

namespace Alice
{

struct FramebufferSpecification
{
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

    virtual uint32_t GetColorAttachmentRendererID() const = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};

} // namespace Alice
