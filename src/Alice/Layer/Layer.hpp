#pragma once

#include "Alice/Events/Event.hpp"
#include "Alice/Core/Timestep.hpp"

namespace Alice
{

class Layer
{
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) {}

    inline const std::string& GetName() const { return m_debug_name; }

protected:
    std::string m_debug_name;
};

} // namespace Alice
