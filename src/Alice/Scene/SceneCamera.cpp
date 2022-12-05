#include "SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Alice
{

SceneCamera::SceneCamera()
{
    RecalculateProjection();
}

void SceneCamera::SetOrthographic(float size, float near_clip, float far_clip)
{
    m_projection_type = ProjectionType::Orthographic;
    m_orthographic_size = size;
    m_orthographic_near = near_clip;
    m_orthographic_far = far_clip;
    RecalculateProjection();
}

void SceneCamera::SetPerspective(float vertical_fov, float near_clip, float far_clip)
{
    m_projection_type = ProjectionType::Perspective;
    m_perspective_fov = vertical_fov;
    m_perspective_near = near_clip;
    m_perspective_far = far_clip;
    RecalculateProjection();
}

void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
{
    m_aspect_ratio = (float)width / (float)height;
    RecalculateProjection();
}

void SceneCamera::RecalculateProjection()
{
    if (m_projection_type == ProjectionType::Perspective)
    {
        m_projection = glm::perspective(m_perspective_fov, m_aspect_ratio, m_perspective_near, m_perspective_far);
    }
    else
    {
        float ortho_left = -m_orthographic_size * m_aspect_ratio * 0.5f;
        float ortho_right = m_orthographic_size * m_aspect_ratio * 0.5f;
        float ortho_bottom = -m_orthographic_size * 0.5f;
        float ortho_top = m_orthographic_size * 0.5f;

        m_projection = glm::ortho(
            ortho_left, ortho_right, ortho_bottom, ortho_top,
            m_orthographic_near, m_orthographic_far
        );
    }
}

// Orthographic Setter
void SceneCamera::SetOrthographicSize(float size)
{
    m_orthographic_size = size;
    RecalculateProjection();
}

void SceneCamera::SetOrthographicNearClip(float near_clip)
{
    m_orthographic_near = near_clip;
    RecalculateProjection();
}

void SceneCamera::SetOrthographicFarClip(float far_clip)
{
    m_orthographic_far = far_clip;
    RecalculateProjection();
}

// Perspective Setter
void SceneCamera::SetPerspectiveVerticalFov(float vertical_fov)
{
    m_perspective_fov = vertical_fov;
    RecalculateProjection();
}

void SceneCamera::SetPerspectiveNearClip(float near_clip)
{
    m_perspective_near = near_clip;
    RecalculateProjection();
}

void SceneCamera::SetPerspectiveFarClip(float far_clip)
{
    m_perspective_far = far_clip;
    RecalculateProjection();
}

void SceneCamera::SetProjectionType(ProjectionType type)
{
    m_projection_type = type;
    RecalculateProjection();
}

} // namespace Alice
