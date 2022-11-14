#include "Buffer.hpp"
#include "Renderer.hpp"
#include "Alice/Log/Log.hpp"
#include "Alice/Platform/OpenGL/OpenGLBuffer.hpp"

namespace Alice
{

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "VertexBuffer::Create: RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return std::make_shared<OpenGLVertexBuffer>(vertices, size);
            break;
        }
    }

    ALICE_ASSERT(false, "VertexBuffer::Create: Unknown RendererApi!");

    return nullptr;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "IndexBuffer::Create: RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
            break;
        }
    }

    ALICE_ASSERT(false, "IndexBuffer::Create: Unknown RendererApi!");

    return nullptr;
}

} // namespace Alice
