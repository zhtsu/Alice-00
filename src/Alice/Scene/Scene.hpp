#pragma once

#include <entt.hpp>
#include "Alice/Core/Timestep.hpp"

namespace Alice
{

class Entity;
class EditorCamera;

class Scene
{
    friend class Entity;
    friend class SceneHierarchyPanel;
    friend class SceneSerializer;

public:
    Scene();
    ~Scene() = default;

    Entity CreateEntity(const std::string& name = std::string());
    void DestroyEntity(Entity entity);

    void OnUpdateEditor(Timestep ts, EditorCamera& camera);
    void OnUpdateRuntime(Timestep ts);
    void OnViewportResize(uint32_t width, uint32_t height);

    inline uint32_t GetAliveEntityCount() const { return m_registry.alive(); }
    inline uint32_t GetEntityCapacity() const { return m_registry.capacity(); }
    inline uint32_t GetRenderedEntitiesCount() const { return m_rendered_entities_count; }

    Entity GetPrimaryCameraEntity();

private:
    template<class T>
    void OnComponentAdded(Entity entity, T& component);

private:
    entt::registry m_registry;
    uint32_t m_viewport_width = 0, m_viewport_height = 0;

    uint32_t m_rendered_entities_count = 0;
};

} // namespace Alice
