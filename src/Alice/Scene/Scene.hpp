#pragma once

#include <entt.hpp>
#include "Alice/Core/Timestep.hpp"

namespace Alice
{

class Entity;

class Scene
{
    friend class Entity;
    friend class SceneHierarchyPanel;
    friend class SceneSerializer;

public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());
    void DestroyEntity(Entity entity);

    void OnUpdate(Timestep ts);
    void OnViewportResize(uint32_t width, uint32_t height);

    inline uint32_t GetAliveEntityCount() { return m_registry.alive(); }
    inline uint32_t GetEntityCapacity() { return m_registry.capacity(); }
    inline uint32_t GetRenderedEntitiesCount() { return m_rendered_entities_count; }

private:
    template<class T>
    void OnComponentAdded(Entity entity, T& component);

private:
    entt::registry m_registry;
    uint32_t m_viewport_width = 0, m_viewport_height = 0;

    uint32_t m_rendered_entities_count = 0;
};

} // namespace Alice
