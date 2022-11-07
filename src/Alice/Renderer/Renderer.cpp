#include "Renderer.hpp"
#include "RenderCommand.hpp"

namespace Alice
{

void Renderer::BeginScene()
{

}

void Renderer::EndScene()
{

}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertex_array)
{
    vertex_array->Bind();
    RenderCommand::DrawIndexed(vertex_array);
}

} // namespace Alice
