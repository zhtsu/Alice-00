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
        if (HasComponent<T>())
        {
            const char* type_name = typeid(T).name();
            ALICE_TRACE("Entity ({}) already has component ({}) !", (int32_t)m_entity_handle, type_name);
            ALICE_ASSERT(false, "");
        }
        T& component = m_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
        m_scene->OnComponentAdded<T>(*this, component);
        return component;
    }

    template<class T>
    T& GetComponent()
    {
        if (!HasComponent<T>())
        {
            const char* type_name = typeid(T).name();
            ALICE_ERROR("Entity ({}) does not have component ({}) !", (int32_t)m_entity_handle, type_name);
            ALICE_ASSERT(false, "");
        }
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
        if (!HasComponent<T>())
        {
            const char* type_name = typeid(T).name();
            ALICE_ERROR("Entity ({}) does not have component ({}) !", (int32_t)m_entity_handle, type_name);
            ALICE_ASSERT(false, "");
        }
        
        m_scene->m_registry.remove<T>(m_entity_handle);
    }

    bool operator==(const Entity& other) const
    {
        return m_entity_handle == other.m_entity_handle && m_scene == other.m_scene;
    }
    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

    uint64_t GetUUID();
    const std::string& GetName();

    operator bool() const { return m_entity_handle != entt::null; }
    operator uint32_t() const { return (uint32_t)m_entity_handle; }
    operator entt::entity() const { return m_entity_handle; }

private:
    entt::entity m_entity_handle{ entt::null };
    Scene* m_scene = nullptr;
};

} // namespace Alice
