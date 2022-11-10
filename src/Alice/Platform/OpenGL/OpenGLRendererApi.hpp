#pragma once

#include "Alice/Renderer/RendererApi.hpp"
#include "Alice/Core/Core.hpp"

namespace Alice
{

class OpenGLRendererApi : public RendererApi
{
public:
    void Init() override;
    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawIndexed(const Ref<VertexArray>& vertex_array) override;
};

} // namespace Alice
