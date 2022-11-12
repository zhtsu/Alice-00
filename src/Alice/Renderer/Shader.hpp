#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Core/Core.hpp"
#include "glm/glm.hpp"

namespace Alice
{

class Shader
{
public:
    ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual const std::string& GetName() const = 0;

    static Ref<Shader> Create(const std::string& filepath);
    static Ref<Shader> Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
};

class ShaderLibrary
{
public:
    void Add(const Ref<Shader>& shader);
    void Add(const std::string& name, const Ref<Shader>& shader);
    Ref<Shader> Load(const std::string& filepath);
    Ref<Shader> Load(const std::string& name, const std::string& filepath);

    Ref<Shader> Get(const std::string& name);

    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, Ref<Shader>> m_shaders;
};

} // namespace Alice
