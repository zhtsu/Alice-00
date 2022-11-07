#pragma once

#include "Alice/PCH.hpp"
#include "RendererApi.hpp"

namespace Alice
{

class Renderer
{
public:
    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray>& vertex_array);

    inline static RendererApi::Api GetApi() { return RendererApi::GetApi();}
};


} // namespace Alice
