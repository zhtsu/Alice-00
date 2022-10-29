#pragma once

#include "Alice/Core/Layer/Layer.hpp"
#include "Alice/Core/Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Alice/Core/Events/KeyEvent.hpp"
#include "Alice/Core/Events/MouseEvent.hpp"
#include "Alice/Core/Events/ApplicationEvent.hpp"

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
