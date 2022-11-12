#include "Shader.hpp"
#include "Alice/Renderer/Renderer.hpp"
#include "Alice/Platform/OpenGL/OpenGLShader.hpp"

namespace Alice
{

Ref<Shader> Shader::Create(const std::string& filepath)
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
            return std::make_shared<OpenGLShader>(filepath);
            break;
        }
    }

    ALICE_ASSERT(false, "Shader::Create: Unknown RendererApi!");

    return nullptr;
}

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src)
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
            return std::make_shared<OpenGLShader>(name, vertex_src, fragment_src);
            break;
        }
    }

    ALICE_ASSERT(false, "Shader::Create: Unknown RendererApi!");

    return nullptr;
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
    ALICE_ASSERT(!Exists(name), "ShaderLibrary::Add: Shader already exists!");
    m_shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    auto& name = shader->GetName();
    Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
{
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
    auto shader = Shader::Create(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name)
{
    ALICE_ASSERT(Exists(name), "ShaderLibrary::Get: Shader not found!");
    return m_shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    return m_shaders.find(name) != m_shaders.end();
}

} // namespace Alice
