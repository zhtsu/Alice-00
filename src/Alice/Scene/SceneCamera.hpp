#pragma once

#include "Alice/Renderer/Camera.hpp"

namespace Alice
{

class SceneCamera : public Camera
{
public:
    enum class ProjectionType { Perspective = 0, Orthographic = 1 };

public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float near_clip, float far_clip);
    void SetPerspective(float vertical_fov, float near_clip, float far_clip);
    void SetViewportSize(uint32_t width, uint32_t height);

    inline float GetOrthographicSize() const { return m_orthographic_size; }
    inline float GetOrthographicNearClip() const { return m_orthographic_near; }
    inline float GetOrthographicFarClip() const { return m_orthographic_far; }
    void SetOrthographicSize(float size);
    void SetOrthographicNearClip(float near_clip);
    void SetOrthographicFarClip(float far_clip);

    inline float GetPerspectiveVerticalFov() const { return m_perspective_fov; }
    inline float GetPerspectiveNearClip() const { return m_perspective_near; }
    inline float GetPerspectiveFarClip() const { return m_perspective_far; }
    void SetPerspectiveVerticalFov(float vertical_fov);
    void SetPerspectiveNearClip(float near_clip);
    void SetPerspectiveFarClip(float far_clip);

    inline ProjectionType GetProjectionType() const { return m_projection_type; }
    void SetProjectionType(ProjectionType type);

private:
    void RecalculateProjection();

private:
    ProjectionType m_projection_type = ProjectionType::Orthographic;

    float m_orthographic_size = 10.0f;
    float m_orthographic_near = -1.0f, m_orthographic_far = 1.0f;

    float m_perspective_fov = glm::radians(45.0f);
    float m_perspective_near = 0.01f, m_perspective_far = 1000.0f;

    float m_aspect_ratio = 0.0f;
};

} // namespace Alice
