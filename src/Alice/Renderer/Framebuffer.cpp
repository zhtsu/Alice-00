#include "Framebuffer.hpp"

#include "Alice/Log/Log.hpp"
#include "Alice/Renderer/Renderer.hpp"
#include "Alice/Platform/OpenGL/OpenGLFramebuffer.hpp"

namespace Alice
{

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "Framebuffer::Create: RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return std::make_shared<OpenGLFramebuffer>(spec);
            break;
        }
    }

    ALICE_ASSERT(false, "Framebuffer::Create: Unknown RendererApi!");

    return nullptr;
}

} // namespace Alice
