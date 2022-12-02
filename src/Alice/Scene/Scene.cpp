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
    auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
    for (auto& entity : group)
    {
        auto&& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
        Renderer2D::DrawQuad(transform, sprite.color);
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
