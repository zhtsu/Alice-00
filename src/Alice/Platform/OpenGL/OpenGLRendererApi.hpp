#pragma once

#include "Alice/Renderer/RendererApi.hpp"

namespace Alice
{

class OpenGLRendererApi : public RendererApi
{
public:
    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array) override;
};

} // namespace Alice
