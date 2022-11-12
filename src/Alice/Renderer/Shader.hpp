#pragma once

#include "Alice/PCH.hpp"

#include "glm/glm.hpp"

namespace Alice
{

class Shader
{
public:
    ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Shader* Create(const std::string& filepath);
    static Shader* Create(const std::string& vertex_src, const std::string& fragment_src);
};

} // namespace Alice
