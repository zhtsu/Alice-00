#pragma once

#include <glm/glm.hpp>
#include "Alice/PCH.hpp"
#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"

namespace Alice
{

struct TagComponent
{
    std::string tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& _tag) : tag(_tag) {}
};

struct TransformComponent
{
    glm::mat4 transform{ 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& _transform) : transform(_transform) {}
    operator glm::mat4&() { return transform; }
    operator const glm::mat4&() const { return transform; }
};

struct SpriteRendererComponent
{
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const glm::vec4& _color) : color(_color) {}
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

} // namespace Alice;
