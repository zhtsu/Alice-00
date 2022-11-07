#include "VertexArray.hpp"
#include "Renderer.hpp"
#include "Alice/Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Alice
{

VertexArray* VertexArray::Create()
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "VertexArray::Create: RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return new OpenGLVertexArray();
            break;
        }
    }

    ALICE_ASSERT(false, "VertexArray::Create: Unknown RendererApi!");

    return nullptr;
}

} // namespace Alice
