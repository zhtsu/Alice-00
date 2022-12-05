#pragma once

#include "Scene.hpp"
#include "Alice/Log/Log.hpp"
#include <entt.hpp>

namespace Alice
{

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, Scene* scene)
        : m_entity_handle(handle), m_scene(scene) {}
    Entity(const Entity& other) = default;

    template<class T, class... Args>
    T& AddComponent(Args&&... args)
    {
        ALICE_ASSERT(!HasComponent<T>(), "Entity already has component!");
        return m_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
    }

    template<class T>
    T& GetComponent()
    {
        ALICE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        return m_scene->m_registry.get<T>(m_entity_handle);
    }

    template<class T>
    bool HasComponent()
    {
        return m_scene->m_registry.all_of<T>(m_entity_handle);
    }

    template<class T>
    void RemoveComponent()
    {
        ALICE_ASSERT(HasComponent<T>(), "Entity does not have component!");
        m_scene->m_registry.remove<T>(m_entity_handle);
    }

    operator bool() const { return m_entity_handle != entt::null; }
    operator uint32_t() const { return (uint32_t)m_entity_handle; }

    bool operator==(const Entity& other) const
    {
        return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene;
    }
    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

private:
    entt::entity m_entity_handle{ entt::null };
    Scene* m_scene = nullptr;
};

} // namespace Alice
