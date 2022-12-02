#pragma once

#include "entt.hpp"
#include "Alice/Core/Timestep.hpp"

namespace Alice
{

class Scene
{
public:
    Scene();
    ~Scene();

    void OnUpdate(Timestep ts);

private:
    entt::registry m_registry;
};

} // namespace Alice
