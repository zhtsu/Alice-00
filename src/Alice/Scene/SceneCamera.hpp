#pragma once

#include "Alice/Renderer/Camera.hpp"

namespace Alice
{

class SceneCamera : public Camera
{
public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float near_clip, float far_clip);
    void SetViewportSize(uint32_t width, uint32_t height);

    inline float GetOrthographicSize() const { return m_orthographic_size; }
    void SetOrthographicSize(float size);

private:
    void RecalculateProjection();

private:
    float m_orthographic_size = 10.0f;
    float m_orthographic_near = -1.0f, m_orthographic_far = 1.0f;

    float m_aspect_ratio = 0.0f;
};

} // namespace Alice
