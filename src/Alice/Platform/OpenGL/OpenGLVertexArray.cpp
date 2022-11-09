#include "OpenGLVertexArray.hpp"
#include "glad/glad.h"

namespace Alice
{

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:     return GL_FLOAT;
        case ShaderDataType::Float2:    return GL_FLOAT;
        case ShaderDataType::Float3:    return GL_FLOAT;
        case ShaderDataType::Float4:    return GL_FLOAT;
        case ShaderDataType::Int:       return GL_INT;
        case ShaderDataType::Int2:      return GL_INT;
        case ShaderDataType::Int3:      return GL_INT;
        case ShaderDataType::Int4:      return GL_INT;
        case ShaderDataType::Mat3:      return GL_FLOAT;
        case ShaderDataType::Mat4:      return GL_FLOAT;
        case ShaderDataType::Bool:      return GL_BOOL;
    }
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &m_renderer_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_renderer_id);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(m_renderer_id);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer)
{
    ALICE_ASSERT(vertex_buffer->GetLayout().GetElements().size(), "OpenGLVertexArray::AddVertexBuffer: VertexBuffer has no layout!");

    glBindVertexArray(m_renderer_id);
    vertex_buffer->Bind();

    uint32_t index = 0;
    const auto& layout = vertex_buffer->GetLayout();
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index, element.GetElementCount(),
            ShaderDataTypeToOpenGLBaseType(element.type),
            element.normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            (const void*)element.offset
        );
        index++;
    }

    m_vertex_buffers.push_back(vertex_buffer);
}

void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer)
{
    glBindVertexArray(m_renderer_id);
    index_buffer->Bind();

    m_index_buffer = index_buffer;
}

} // namespace Alice
