#pragma once

#include "Alice/Renderer/Buffer.hpp"

namespace Alice
{

///
/// OpenGL VBO
///
class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

    virtual void SetData(const void* data, uint32_t size) override;

    virtual void SetLayout(const BufferLayout& layout) { m_layout = layout; }
    virtual const BufferLayout& GetLayout() const { return m_layout; }

private:
    uint32_t m_renderer_id;
    BufferLayout m_layout;

};

///
/// OpenGL EBO
///
class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const;
    virtual void Unbind() const;

    virtual uint32_t GetCount() const { return m_count; };

private:
    uint32_t m_renderer_id;
    uint32_t m_count;
};

} // namespace Alice
