#include "OpenGLShader.hpp"
#include "Alice/Log/Log.hpp"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include "Alice/Common/FileHelper.hpp"

namespace Alice
{

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;
    
    ALICE_ASSERT(false, "Unknown shader type!");

    return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
    std::string source = FileHelper::Read(filepath);
    auto shader_sources = PreProcess(source);
    Compile(shader_sources);
}

OpenGLShader::OpenGLShader(const std::string& vertex_src, const std::string& fragment_src)
{
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertex_src;
    sources[GL_FRAGMENT_SHADER] = fragment_src;
    Compile(sources);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_renderer_id);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_renderer_id);
}

void OpenGLShader::Unbind() const
{
    glUseProgram(0);
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
{
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
{
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
{
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3 matrix)
{
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4 matrix)
{
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
{
    std::unordered_map<GLenum, std::string> shader_sources;

    const char* type_token = "#type";
    size_t type_token_length = strlen(type_token);
    size_t pos = source.find(type_token, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        ALICE_ASSERT(eol != std::string::npos, "OpenGLShader::PreProcess: Syntax error!");
        size_t begin = pos + type_token_length + 1;
        std::string type = source.substr(begin, eol - begin);
        ALICE_ASSERT(
            type == "vertex" || type == "fragment" || type == "pixel",
            "Invalid shader type specified"
        );

        size_t next_line_pos = source.find_first_not_of("\r\n", eol);
        pos = source.find(type_token, next_line_pos);
        shader_sources[ShaderTypeFromString(type)] = source.substr(
            next_line_pos, pos - (next_line_pos)
        );
    }

    return shader_sources;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shader_sources)
{
    GLuint program = glCreateProgram();
    std::vector<GLenum> gl_shader_ids(shader_sources.size());
    for (auto& kv : shader_sources)
    {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar *source_cstr = (const GLchar *)source.c_str();
        glShaderSource(shader, 1, &source_cstr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
            
            glDeleteShader(shader);

            ALICE_ERROR("{}", infoLog.data());
            ALICE_ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(program, shader);
        gl_shader_ids.push_back(shader);
    }

    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        
        glDeleteProgram(program);
        for (auto id : gl_shader_ids)
        {
            glDeleteShader(id);
        }
        
        ALICE_ERROR("{}", infoLog.data());
        ALICE_ASSERT(false, "Shader link failure!");
        return;
    }

    for (auto id : gl_shader_ids)
    {
        glDetachShader(program, id);
    }

    m_renderer_id = program;
}

} // namespace Alice
