#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"
#include "Alice/Core/Timestep.hpp"
#include "Alice/Events/Event.hpp"
#include "Alice/Events/MouseEvent.hpp"

namespace Alice
{

class EditorCamera : public Camera
{
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& event);

    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;
    glm::quat GetOrientation() const;
    
    void SetViewportSize(float width, float height);

    inline float GetDistance() const { return m_distance; }
    inline void SetDistance(float distance) { m_distance = distance;}

    const glm::mat4& GetViewMatrix() const { return m_view_matrix; }
    glm::mat4 GetViewProjection() const { return m_projection * m_view_matrix; }

    const glm::vec3& GetPosition() const { return m_position; }

    float GetPitch() const { return m_pitch; }
    float GetYaw() const { return m_yaw; }

private:
    void UpdateProjection();
    void UpdateView();

    bool OnMouseScroll(MouseScrolledEvent& event);

    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    // @TODO:
    // Too large zoom will cause the viewport to disappear
    void MouseZoom(float delta);

    glm::vec3 CalculatePosition() const;

    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

private:
    float m_fov = 45.0f, m_aspect_ratio = 1.778f, m_near_clip = 0.1f, m_far_clip = 1000.0f;

    glm::mat4 m_view_matrix;
    glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_focal_point = { 0.0f, 0.0f, 0.0f };

    glm::vec2 m_initial_mouse_position = { 0.0f, 0.0f };

    float m_distance = 10.0f;
    float m_pitch = 0.0f, m_yaw = 0.0f;

    float m_viewport_width = 1280, m_viewport_height = 720;
};

} // namespace Alice
