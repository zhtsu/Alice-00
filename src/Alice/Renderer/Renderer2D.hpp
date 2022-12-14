#pragma once

#include "Camera.hpp"
#include "OrthographicCamera.hpp"
#include "Texture.hpp"
#include "SubTexture2D.hpp"
#include "EditorCamera.hpp"

namespace Alice
{

struct SpriteRendererComponent;

class Renderer2D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& camera, const glm::mat4& transform);
    static void BeginScene(const EditorCamera& camera);
    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();
    static void Flush();

    static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entity_id = -1);
    static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& color = glm::vec4(1.0f), int entity_id = -1);

    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling_factor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

    static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& sprite, int entity_id);

    struct Statistics
    {
        uint32_t draw_calls = 0;
        uint32_t quad_count = 0;

        uint32_t GetTotalVertexCount() { return quad_count * 4; }
        uint32_t GetTotalIndexCount() { return quad_count * 6; }
    };

    static void ResetStats();
    static Statistics GetStats();

private:
    static void StartBatch();
    static void NextBatch();
};

} // namespace Alice
