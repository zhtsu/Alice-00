#pragma once

#include "Alice/PCH.hpp"
#include "RendererApi.hpp"
#include "OrthographicCamera.hpp"
#include "Shader.hpp"
#include "glm/glm.hpp"
#include "Alice/Core/Base.hpp"

namespace Alice
{

class Renderer
{
public:
    static void Init();
    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(OrthographicCamera& camera);
    static void EndScene();

    inline static RendererApi::Api GetApi() { return RendererApi::GetApi();}

private:
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    static SceneData* s_scene_data;
};


} // namespace Alice
