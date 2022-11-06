#pragma once

#include "Alice/Renderer/VertexArray.hpp"

namespace Alice
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) override;
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) override;

    std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() override
    {
        return m_vertex_buffers;
    }

    std::shared_ptr<IndexBuffer>& GetIndexBuffer() override
    {
        return m_index_buffer;
    }

private:
    uint32_t m_renderer_id;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertex_buffers;
    std::shared_ptr<IndexBuffer> m_index_buffer;
};

} // namespace Alice
