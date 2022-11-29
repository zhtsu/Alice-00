#pragma once

#include "Alice/Renderer/VertexArray.hpp"
#include "Alice/Core/Base.hpp"

namespace Alice
{

//
// VAO
//
class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) override;
    void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) override;

    std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override
    {
        return m_vertex_buffers;
    }

    Ref<IndexBuffer>& GetIndexBuffer() override
    {
        return m_index_buffer;
    }

private:
    uint32_t m_renderer_id;
    std::vector<Ref<VertexBuffer>> m_vertex_buffers;
    Ref<IndexBuffer> m_index_buffer;
};

} // namespace Alice
