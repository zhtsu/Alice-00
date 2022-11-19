#pragma once

#include "RendererApi.hpp"

namespace Alice
{

//
// 平台无关的渲染命令
// 这是一个静态工具类
//
class RenderCommand
{
public:
    inline static void Init()
    {
        s_renderer_api->Init();
    }

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_renderer_api->SetViewport(x, y, width, height);
    }

    inline static void SetClearColor(const glm::vec4& color)
    {
        s_renderer_api->SetClearColor(color);
    }

    inline static void Clear()
    {
        s_renderer_api->Clear();
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count)
    {
        s_renderer_api->DrawIndexed(vertex_array, index_count);
    }

private:
    static RendererApi* s_renderer_api;
};

} // namespace Alice
