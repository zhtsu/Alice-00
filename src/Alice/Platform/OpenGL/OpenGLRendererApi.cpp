#include "OpenGLRendererApi.hpp"
#include "glad/glad.h"

namespace Alice
{

void OpenGLRendererApi::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererApi::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererApi::DrawIndexed(const Ref<VertexArray>& vertex_array)
{
    glDrawElements(
        GL_TRIANGLES,
        vertex_array->GetIndexBuffer()->GetCount(),
        GL_UNSIGNED_INT,
        nullptr
    );
}

} // namespace Alice
