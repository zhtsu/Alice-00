#pragma once

#include "glm/glm.hpp"

namespace Alice
{

//
// 用于 2D 的正交摄像机
// 平台无关
//
class OrthographicCamera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);
    void SetProjection(float left, float right, float bottom, float top);

    const glm::vec3& GetPosition() const { return m_position; }
    void SetPosition(const glm::vec3& position)
    {
        m_position = position;
        RecalculateViewMatrix();
    }

    float GetRotation() const { return m_rotation; }
    void SetRotation(float rotation)
    {
        m_rotation = rotation;
        RecalculateViewMatrix();
    }

    const glm::mat4& GetProjectionMatrix() const { return m_projection_matrix; }
    const glm::mat4& GetViewMatrix() const { return m_view_matrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_view_projection_matrix; }

private:
    // 在每一次摄像机状态更新后重新计算 View 矩阵和 Projection 矩阵
    void RecalculateViewMatrix();

private:
    glm::mat4 m_projection_matrix;
    glm::mat4 m_view_matrix;
    glm::mat4 m_view_projection_matrix;

    glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
    float m_rotation = 0.0f;
};

} // namespace Alice
