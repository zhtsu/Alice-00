#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Alice/PCH.hpp"
#include "Alice/Core/Base.hpp"
#include "Alice/Renderer/Texture.hpp"
#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"
#include "Alice/Core/UUID.hpp"

namespace Alice
{

struct IDComponent
{
    UUID id;

    IDComponent() = default;
    IDComponent(const IDComponent&) = default;
};

struct TagComponent
{
    std::string tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& _tag) : tag(_tag) {}
};

struct TransformComponent
{
    glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& _translation) : translation(_translation) {}
    
    glm::mat4 GetTransform() const
    {
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 rotation_matrix = glm::toMat4(glm::quat(rotation));
        glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);

        return translation_matrix * rotation_matrix * scale_matrix;
    }
};

struct SpriteRendererComponent
{
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    Ref<Texture2D> texture;
    float tiling_factor = 1.0f;

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& _color) : color(_color) {}
};

struct CircleRendererComponent
{
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    float radius = 0.5f;
    float thickness = 1.0f;
    float fade = 0.005f;

    CircleRendererComponent() = default;
    CircleRendererComponent(const CircleRendererComponent&) = default;
};

struct CameraComponent
{
    Alice::SceneCamera camera;
    bool is_primary = true;
    bool is_fixed_aspect_ratio = false;

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
};

struct NativeScriptComponent
{
    ScriptableEntity* instance = nullptr;

    ScriptableEntity* (*InstantiateScript) ();
    void (*DestroyScript) (NativeScriptComponent*);

    template<class T>
    void Bind()
    {
        InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
        DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
    }
};

struct Rigidbody2DComponent
{
    enum class BodyType
    {
        Static = 0, Dynamic, Kinematic
    };

    BodyType type = BodyType::Static;
    bool fixed_rotation = false;

    void* runtime_body = nullptr;

    Rigidbody2DComponent() = default;
    Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
};

struct BoxCollider2DComponent
{
    glm::vec2 offset = { 0.0f, 0.0f };
    glm::vec2 size = { 0.5f, 0.5f };

    float density = 1.0f;
    float friction = 0.5f;
    float restitution = 0.0f;
    float restitution_threshold = 0.5f;

    void* runtime_fixture = nullptr;

    BoxCollider2DComponent() = default;
    BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};

} // namespace Alice;
