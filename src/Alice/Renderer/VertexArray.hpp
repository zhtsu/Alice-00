#pragma once

#include "Buffer.hpp"

namespace Alice
{

class VertexArray
{
public:
    virtual ~VertexArray() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) = 0;

    virtual std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() = 0;
    virtual std::shared_ptr<IndexBuffer>& GetIndexBuffer() = 0;

    static VertexArray* Create();
};

} // namespace Alice
