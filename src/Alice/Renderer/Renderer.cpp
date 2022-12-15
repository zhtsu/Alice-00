#include "Renderer.hpp"
#include "RenderCommand.hpp"

#include "Alice/Platform/OpenGL/OpenGLShader.hpp"
#include "Alice/PCH.hpp"
#include "Alice/Renderer/Renderer2D.hpp"

namespace Alice
{

Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData();

void Renderer::Init()
{
    RenderCommand::Init();
    Renderer2D::Init();
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

} // namespace Alice
