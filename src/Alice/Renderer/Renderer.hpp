#pragma once

namespace Alice
{

enum class RendererApi
{
    None = 0,
    OpenGL = 1
};

class Renderer
{
public:
    inline static RendererApi GetApi() { return s_renderer_api; }

private:
    static RendererApi s_renderer_api;
};


} // namespace Alice
