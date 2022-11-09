#pragma once

#include "Alice/PCH.hpp"
#include "RendererApi.hpp"
#include "OrthographicCamera.hpp"
#include "Shader.hpp"
#include "glm/glm.hpp"
#include "Alice/Core/Core.hpp"

namespace Alice
{

class Renderer
{
public:
    static void BeginScene(OrthographicCamera& camera);
    static void EndScene();

    static void Submit(
        const Ref<VertexArray>& vertex_array,
        const Ref<Shader>& shader,
        const glm::mat4& transform = glm::mat4(1.0f)
    );

    inline static RendererApi::Api GetApi() { return RendererApi::GetApi();}

private:
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* s_scene_data;
};


} // namespace Alice
