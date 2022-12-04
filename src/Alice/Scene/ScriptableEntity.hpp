#pragma once

#include "Entity.hpp"

namespace Alice
{

class ScriptableEntity
{
    friend class Scene;

public:
    virtual ~ScriptableEntity() {}

    template<class T>
    T& GetComponent()
    {
        return m_entity.GetComponent<T>();
    }

protected:
    virtual void OnCreate() {}
    virtual void OnDestroy() {}
    virtual void OnUpdate(Timestep ts) {}

private:
    Entity m_entity;
};

} // namespace Alice
