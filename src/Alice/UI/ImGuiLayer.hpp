#pragma once

#include "Alice/Layer/Layer.hpp"
#include "Alice/Events/KeyEvent.hpp"
#include "Alice/Events/MouseEvent.hpp"
#include "Alice/Events/ApplicationEvent.hpp"

namespace Alice
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    void OnUpdate();
    void OnEvent(Event& event);

private:
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
    bool OnMouseMovedEvent(MouseMovedEvent& event);
    bool OnMouseScrolledEvent(MouseScrolledEvent& event);
    bool OnKeyPressedEvent(KeyPressedEvent& event);
    bool OnKeyReleasedEvent(KeyReleasedEvent& event);
    bool OnKeyTypedEvent(KeyTypedEvent& event);
    bool OnWindowResizedEvent(WindowResizeEvent& event);

private:
    float m_time;
};

} // namespace Alice
