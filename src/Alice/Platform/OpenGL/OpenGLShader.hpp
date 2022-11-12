#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Renderer/Shader.hpp"
#include "glad/glad.h"

namespace Alice
{

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& vertex_src, const std::string& fragment_src);
    virtual ~OpenGLShader();

    void Bind() const override;
    void Unbind() const override;

    void UploadUniformInt(const std::string& name, int value);
    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
    void UploadUniformMat3(const std::string& name, const glm::mat3 matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4 matrix);

private:
    std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shader_sources);

private:
    uint32_t m_renderer_id;
};

} // namespace Alice
