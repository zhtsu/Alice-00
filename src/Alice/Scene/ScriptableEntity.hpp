#pragma once

#include "Entity.hpp"

namespace Alice
{

class ScriptableEntity
{
    friend class Scene;

public:
    template<class T>
    T& GetComponent()
    {
        return m_entity.GetComponent<T>();
    }

private:
    Entity m_entity;
};

} // namespace Alice
