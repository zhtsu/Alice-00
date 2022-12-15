#pragma once

#include "Alice/Alice.hpp"
#include "Panel/SceneHierarchyPanel.hpp"

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
    bool OnKeyPressed(KeyPressedEvent& event);

    void NewScene();
    void OpenScene();
    void SaveSceneAs();

private:
    Entity m_camera_entity;
    Entity m_hovered_entity;

    OrthographicCameraController m_camera_controller;
    EditorCamera m_editor_camera;
    
    Ref<Framebuffer> m_framebuffer;

    bool m_viewport_focused = false, m_viewport_hovered = false;
    glm::vec2 m_viewport_size = { 0.0f, 0.0f };
    glm::vec2 m_viewport_bounds[2];

    Ref<Scene> m_active_scene;

    int m_gizmo_type = -1;

    // Panels
    SceneHierarchyPanel m_scene_hierarchy_panel;
};

} // namespace Alice
