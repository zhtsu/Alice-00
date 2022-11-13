#pragma once

#include "Alice/Renderer/OrthographicCamera.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Events/MouseEvent.hpp"
#include "Timestep.hpp"
#include "glm/glm.hpp"

namespace Alice
{

class OrthographicCameraController
{
public:
    OrthographicCameraController(float aspect_ratio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& event);

    OrthographicCamera& GetCamera() { return m_camera; }
    const OrthographicCamera& GetCamera() const { return m_camera; }

    float GetZoomLevel() const { return m_zoom_level; }
    void SetZoomLevel(float zoom_level) { m_zoom_level = zoom_level; }

private:
    bool OnMouseScrolled(MouseScrolledEvent& event);
    bool OnWindowResized(WindowResizeEvent& event);

private:
    float m_aspect_ratio;
    float m_zoom_level = 1.0f;
    OrthographicCamera m_camera;

    bool m_rotation;

    glm::vec3 m_camera_position = { 0.0f, 0.0f, 0.0f };
    float m_camera_rotation = 0.0f;
    float m_camera_translation_speed = 2.0f;
    float m_camera_rotation_speed = 50.0f;
};

} // namespace Alice
