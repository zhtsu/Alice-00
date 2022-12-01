#pragma once

#include "entt.hpp"

namespace Alice
{

class Scene
{
public:
    Scene();
    ~Scene();

private:
    entt::registry m_registry;
};

} // namespace Alice
