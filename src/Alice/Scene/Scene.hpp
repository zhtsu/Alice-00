#pragma once

#include <entt.hpp>
#include "Alice/Core/Timestep.hpp"

namespace Alice
{

class Entity;

class Scene
{
    friend class Entity;

public:
    Scene();
    ~Scene();

    Entity CreateEntity(const std::string& name = std::string());

    void OnUpdate(Timestep ts);
    void OnViewportResize(uint32_t width, uint32_t height);

private:
    entt::registry m_registry;
    uint32_t m_viewport_width = 0, m_viewport_height = 0;
};

} // namespace Alice
