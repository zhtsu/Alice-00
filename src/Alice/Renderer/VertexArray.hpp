#pragma once

#include "Buffer.hpp"
#include "Alice/Core/Core.hpp"

namespace Alice
{

//
// 平台无关的顶点数组对象 VAO
//
class VertexArray
{
public:
    virtual ~VertexArray() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
    virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) = 0;

    virtual std::vector<Ref<VertexBuffer>>& GetVertexBuffers() = 0;
    virtual Ref<IndexBuffer>& GetIndexBuffer() = 0;

    static Ref<VertexArray> Create();
};

} // namespace Alice
