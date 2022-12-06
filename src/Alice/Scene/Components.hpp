#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
    glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& _translation) : translation(_translation) {}
    
    glm::mat4 GetTransform() const
    {
        glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
            * glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
            * glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });
        glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);

        return translation_matrix * rotation_matrix * scale_matrix;
    }
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
