#include "Renderer.hpp"
#include "RenderCommand.hpp"

#include "Alice/Platform/OpenGL/OpenGLShader.hpp"
#include "Alice/PCH.hpp"

namespace Alice
{

Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData();

void Renderer::Init()
{
    RenderCommand::Init();
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::BeginScene(OrthographicCamera& camera)
{
    s_scene_data->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene()
{

}

void Renderer::Submit(
    const Ref<VertexArray>& vertex_array,
    const Ref<Shader>& shader,
    const glm::mat4& transform
)
{
    shader->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(shader)
        ->UploadUniformMat4("u_ViewProjection", s_scene_data->ViewProjectionMatrix);
    std::dynamic_pointer_cast<OpenGLShader>(shader)
        ->UploadUniformMat4("u_Transform", transform);

    vertex_array->Bind();
    RenderCommand::DrawIndexed(vertex_array);
}

} // namespace Alice
