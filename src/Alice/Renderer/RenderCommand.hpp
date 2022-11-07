#pragma once

#include "RendererApi.hpp"

namespace Alice
{

class RenderCommand
{
public:
    inline static void SetClearColor(const glm::vec4& color)
    {
        s_renderer_api->SetClearColor(color);
    }

    inline static void Clear()
    {
        s_renderer_api->Clear();
    }

    inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array)
    {
        s_renderer_api->DrawIndexed(vertex_array);
    }

private:
    static RendererApi* s_renderer_api;
};

} // namespace Alice
