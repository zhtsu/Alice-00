#include "Shader.hpp"
#include "Alice/Renderer/Renderer.hpp"
#include "Alice/Platform/OpenGL/OpenGLShader.hpp"

namespace Alice
{

Shader* Shader::Create(const std::string& filepath)
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "Shader::Create: RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return new OpenGLShader(filepath);
            break;
        }
    }

    ALICE_ASSERT(false, "Shader::Create: Unknown RendererApi!");

    return nullptr;
}

Shader* Shader::Create(const std::string& vertex_src, const std::string& fragment_src)
{
    switch (Renderer::GetApi())
    {
        case RendererApi::Api::None:
        {
            ALICE_ASSERT(false, "Shader::Create: RendererApi::None is currently not supported!");
            break;
        }

        case RendererApi::Api::OpenGL:
        {
            return new OpenGLShader(vertex_src, fragment_src);
            break;
        }
    }

    ALICE_ASSERT(false, "Shader::Create: Unknown RendererApi!");

    return nullptr;
}

} // namespace Alice
