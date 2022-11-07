#include "RenderCommand.hpp"
#include "Alice/Platform/OpenGL/OpenGLRendererApi.hpp"

namespace Alice
{

RendererApi* RenderCommand::s_renderer_api = new OpenGLRendererApi();

} // namespace Alice
