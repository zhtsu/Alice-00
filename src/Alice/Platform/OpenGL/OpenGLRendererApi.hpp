#pragma once

#include "Alice/Renderer/RendererApi.hpp"
#include "Alice/Core/Base.hpp"

namespace Alice
{

class OpenGLRendererApi : public RendererApi
{
public:
    void Init() override;
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count = 0) override;
};

} // namespace Alice
