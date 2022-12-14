#pragma once

#include "Alice/Alice.hpp"
#include "Panel/SceneHierarchyPanel.hpp"
#include "Panel/ContentBrowserPanel.hpp"

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
    bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

    void NewScene();
    void OpenScene();
    void OpenScene(const std::filesystem::path& path);
    void SaveScene();
    void SaveSceneAs();

    void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

    void OnScenePlay();
    void OnSceneStop();

    void OnDuplicateEntity();

    void PutToolbar();

private:
    Entity m_hovered_entity;

    OrthographicCameraController m_camera_controller;
    EditorCamera m_editor_camera;
    
    Ref<Framebuffer> m_framebuffer;

    bool m_viewport_focused = false, m_viewport_hovered = false;
    glm::vec2 m_viewport_size = { 0.0f, 0.0f };
    glm::vec2 m_viewport_bounds[2];

    Ref<Scene> m_active_scene, m_editor_scene;
    std::filesystem::path m_editor_scene_path;

    int m_gizmo_type = -1;

    // Panels
    SceneHierarchyPanel m_scene_hierarchy_panel;
    ContentBrowserPanel m_content_browser_panel;

    // Toolbar
    enum class SceneState
    {
        Edit = 0, Play = 1
    };

    SceneState m_scene_state = SceneState::Edit;
    
    Ref<Texture2D> m_icon_play, m_icon_pause;
};

} // namespace Alice
