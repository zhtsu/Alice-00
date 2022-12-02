#pragma once

#include "Alice/Alice.hpp"

namespace Alice
{

class EditorLayer : public Layer
{
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& event) override;

private:
    Entity m_square_entity;

    OrthographicCameraController m_camera_controller;
    
    Ref<Framebuffer> m_framebuffer;

    bool m_viewport_focused = false, m_viewport_hovered = false;;
    glm::vec2 m_viewport_size = { 0.0f, 0.0f };

    Ref<Scene> m_active_scene;
};

} // namespace Alice
