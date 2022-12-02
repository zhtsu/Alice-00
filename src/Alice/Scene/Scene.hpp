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

private:
    entt::registry m_registry;
};

} // namespace Alice
