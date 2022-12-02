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

    operator bool() const { return m_scene->m_registry.valid(m_entity_handle); }

private:
    entt::entity m_entity_handle{ 0 };
    Scene* m_scene = nullptr;
};

} // namespace Alice
