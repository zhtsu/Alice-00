#include "Scene.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "Alice/Renderer/Renderer2D.hpp"

namespace Alice
{

Scene::Scene()
{

}

Scene::~Scene()
{
    
}

void Scene::OnUpdate(Timestep ts)
{
    Camera* main_camera_ptr = nullptr;
    glm::mat4* main_camera_transform_ptr = nullptr;
    {
        auto group = m_registry.view<TransformComponent, CameraComponent>();
        for (auto entity : group)
        {
            auto&& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
            if (camera.is_primary)
            {
                main_camera_ptr = &camera.camera;
                main_camera_transform_ptr = &transform.transform;
                break;
            }
        }
    }

    if (main_camera_ptr != nullptr)
    {
        Renderer2D::BeginScene(main_camera_ptr->GetProjection(), *main_camera_transform_ptr);

        auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto& entity : group)
        {
            auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform, sprite.color);
        }

        Renderer2D::EndScene();
    }
}

Entity Scene::CreateEntity(const std::string& name)
{
    Entity entity = { m_registry.create(), this };
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.tag = name.empty() ? "Entity" : name;
    return entity;
}

} // namespace Alice
