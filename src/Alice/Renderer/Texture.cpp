#include "Texture.hpp"

#include "Alice/Renderer/Renderer.hpp"
#include "Alice/Platform/OpenGL/OpenGLTexture.hpp"

namespace Alice
{

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "Texture2D::Create(uint, uint): RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return CreateRef<OpenGLTexture2D>(width, height);
            break;
        }
    }

    ALICE_ASSERT(false, "Texture2D::Create(uint, uint): Unknown RendererApi!");

    return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::string& path)
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "Texture2D::Create: RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return CreateRef <OpenGLTexture2D>(path);
            break;
        }
    }

    ALICE_ASSERT(false, "Texture2D::Create: Unknown RendererApi!");

    return nullptr;
}

} // namespace Alice
