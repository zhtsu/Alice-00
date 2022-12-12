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
    // Update scripts
    {
        m_registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc)
        {
            if (nsc.instance == nullptr)
            {
                nsc.instance = nsc.InstantiateScript();
                nsc.instance->m_entity = Entity{ entity, this };
                nsc.instance->OnCreate();
            }

            nsc.instance->OnUpdate(ts);
        });
    }

    // Render
    Camera* main_camera_ptr = nullptr;
    glm::mat4 main_camera_transform(1.0f);
    {
        auto&& view = m_registry.view<TransformComponent, CameraComponent>();
        for (auto& entity : view)
        {
            auto&& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
            if (camera.is_primary)
            {
                main_camera_ptr = &camera.camera;
                main_camera_transform = transform.GetTransform();
                break;
            }
        }
    }

    m_rendered_entities_count = 0;
    if (main_camera_ptr != nullptr)
    {
        Renderer2D::BeginScene(main_camera_ptr->GetProjection(), main_camera_transform);

        auto&& group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto& entity : group)
        {
            auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform.GetTransform(), sprite.color);
            m_rendered_entities_count++;
        }

        Renderer2D::EndScene();
    }
}

void Scene::OnViewportResize(uint32_t width, uint32_t height)
{
    m_viewport_width = width;
    m_viewport_height = height;

    auto&& view = m_registry.view<CameraComponent>();    
    for (const auto& entity : view)
    {
        auto& camera_component = view.get<CameraComponent>(entity);
        if (!camera_component.is_fixed_aspect_ratio)
            camera_component.camera.SetViewportSize(width, height);
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

void Scene::DestroyEntity(Entity entity)
{
    m_registry.destroy(entity);
}

Entity Scene::GetPrimaryCameraEntity()
{
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view)
    {
        const auto& camera = view.get<CameraComponent>(entity);
        if (camera.is_primary)
            return Entity{ entity, this };
    }
    return {};
}

template<>
void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
{
    
}

template<>
void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
{
    component.camera.SetViewportSize(m_viewport_width, m_viewport_height);
}

template<>
void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
{
    
}

template<>
void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
{

}

template<>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
{

}

} // namespace Alice
