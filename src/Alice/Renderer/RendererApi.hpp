#pragma once

#include "Alice/PCH.hpp"
#include "VertexArray.hpp"
#include "glm/glm.hpp"

namespace Alice
{

class RendererApi
{
public:
    enum class Api
    {
        None = 0,
        OpenGL = 1
    };

public:
    virtual void Init() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0) = 0;

    inline static Api GetApi() { return s_api; }

private:
    static Api s_api;
};

} // namespace Alice
