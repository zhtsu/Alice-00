#include "Scene.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include "Alice/Renderer/Renderer2D.hpp"
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace Alice
{

static b2BodyType ToBox2DType(Rigidbody2DComponent::BodyType body_type)
{
    switch (body_type)
    {
        case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
        case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
        case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
    }

    ALICE_ASSERT(false, "Invalid body type!");

    return b2_staticBody;
}

Scene::Scene()
{

}

template<class ComponentType>
static void CopyComponent(entt::registry& src, entt::registry& dst, const std::unordered_map<UUID, entt::entity>& entt_map)
{
    auto view = src.view<ComponentType>();
    for (auto entity : view)
    {
        UUID uuid = src.get<IDComponent>(entity).id;
        ALICE_ASSERT(entt_map.find(uuid) != entt_map.end(), "Entity not exists in registry!");
        entt::entity dst_entity = entt_map.at(uuid);

        auto& component = src.get<ComponentType>(entity);
        dst.emplace_or_replace<ComponentType>(dst_entity, component);
    }
}

template<class ComponentType>
static void CopyComponentIfExists(Entity src, Entity dst)
{
    if (src.HasComponent<ComponentType>())
        dst.AddOrReplaceComponent<ComponentType>(src.GetComponent<ComponentType>());
}

Ref<Scene> Scene::Copy(Ref<Scene> other)
{
    Ref<Scene> new_scene = CreateRef<Scene>();

    new_scene->m_viewport_width = other->m_viewport_width;
    new_scene->m_viewport_height = other->m_viewport_height;

    std::unordered_map<UUID, entt::entity> entt_map;
    auto& src_scene_registry = other->m_registry;
    auto& dst_scene_registry = new_scene->m_registry;

    // Copy entities
    auto id_view = src_scene_registry.view<IDComponent>();
    for (auto entity : id_view)
    {
        UUID uuid = src_scene_registry.get<IDComponent>(entity).id;
        const auto& name = src_scene_registry.get<TagComponent>(entity).tag;
        Entity new_entity = new_scene->CreateEntityWithUUID(uuid, name);
        entt_map[uuid] = (entt::entity)new_entity;
    }

    // Copy components
    CopyComponent<TransformComponent>(src_scene_registry, dst_scene_registry, entt_map);
    CopyComponent<SpriteRendererComponent>(src_scene_registry, dst_scene_registry, entt_map);
    CopyComponent<CameraComponent>(src_scene_registry, dst_scene_registry, entt_map);
    CopyComponent<NativeScriptComponent>(src_scene_registry, dst_scene_registry, entt_map);
    CopyComponent<Rigidbody2DComponent>(src_scene_registry, dst_scene_registry, entt_map);
    CopyComponent<BoxCollider2DComponent>(src_scene_registry, dst_scene_registry, entt_map);

    return new_scene;
}

void Scene::DuplicateEntity(Entity entity)
{
    std::string name = entity.GetName();
    Entity new_entity = CreateEntity(name);

    CopyComponentIfExists<TransformComponent>(entity, new_entity);
    CopyComponentIfExists<SpriteRendererComponent>(entity, new_entity);
    CopyComponentIfExists<CameraComponent>(entity, new_entity);
    CopyComponentIfExists<NativeScriptComponent>(entity, new_entity);
    CopyComponentIfExists<Rigidbody2DComponent>(entity, new_entity);
    CopyComponentIfExists<BoxCollider2DComponent>(entity, new_entity);
}

void Scene::OnRuntimeStart()
{
    m_physics_world = new b2World({ 0.0f, -9.8f });

    auto view = m_registry.view<Rigidbody2DComponent>();
    for (auto entity_id : view)
    {
        Entity entity = { entity_id, this };
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

        b2BodyDef body_def;
        body_def.type = ToBox2DType(rb2d.type);
        body_def.position.Set(transform.translation.x, transform.translation.y);
        body_def.angle = transform.rotation.z;

        b2Body* body = m_physics_world->CreateBody(&body_def);
        body->SetFixedRotation(rb2d.fixed_rotation);
        rb2d.runtime_body = body;

        if (entity.HasComponent<BoxCollider2DComponent>())
        {
            auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

            b2PolygonShape box_shape;
            box_shape.SetAsBox(bc2d.size.x * transform.scale.x, bc2d.size.y * transform.scale.y);
            
            b2FixtureDef fixture_def;
            fixture_def.shape = &box_shape;
            fixture_def.density = bc2d.density;
            fixture_def.friction = bc2d.friction;
            fixture_def.restitution = bc2d.restitution;
            fixture_def.restitutionThreshold = bc2d.restitution_threshold;
            body->CreateFixture(&fixture_def);
        }
    }
}

void Scene::OnRuntimeStop()
{
    delete m_physics_world;
    m_physics_world = nullptr;
}

void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
{
    Renderer2D::BeginScene(camera);

    m_rendered_entities_count = 0;

    {
        auto&& group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto& entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
            m_rendered_entities_count++;
        }
    }

    // @TODO: Fail to render circle
    {
        auto&& view = m_registry.view<TransformComponent, CircleRendererComponent>();
        for (auto& entity : view)
        {
            auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
            Renderer2D::DrawCircle(transform.GetTransform(), circle.color, circle.thickness, circle.fade, (int)entity);
            m_rendered_entities_count++;
        }
    }

    Renderer2D::EndScene();
}

void Scene::OnUpdateRuntime(Timestep ts)
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

    // Physics
    {
        const int32_t velocity_iterations = 6;
        const int32_t position_iterations = 2;
        m_physics_world->Step(ts, velocity_iterations, position_iterations);

        auto view = m_registry.view<Rigidbody2DComponent>();
        for (auto entity_id : view)
        {
            Entity entity = { entity_id, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

            b2Body* body = (b2Body*)rb2d.runtime_body;
            const auto& position = body->GetPosition();
            transform.translation.x = position.x;
            transform.translation.y = position.y;
            transform.rotation.z = body->GetAngle();
        }
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
    return CreateEntityWithUUID(UUID(), name);
}

Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
{
    Entity entity = { m_registry.create(), this };
    entity.AddComponent<IDComponent>(uuid);
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
void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
{
    
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
void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
{
}

template<>
void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
{
}

template<>
void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
{
}

template<>
void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
{
}

} // namespace Alice
