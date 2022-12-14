#include "EditorCamera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Alice/Core/Input.hpp"
#include "Alice/Core/KeyCodes.hpp"
#include "Alice/Core/MouseButtonCodes.hpp"

namespace Alice
{

EditorCamera::EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip)
    : m_fov(fov), m_aspect_ratio(aspect_ratio), m_near_clip(near_clip), m_far_clip(far_clip),
    Camera(glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip))
{
    UpdateView();
}

void EditorCamera::OnUpdate(Timestep ts)
{
    if (Input::IsKeyPressed(ALICE_KEY_LEFT_ALT))
    {
        const glm::vec2 mouse{ Input::GetMouseX(), Input::GetMouseY() };
        glm::vec2 delta = (mouse - m_initial_mouse_position) * 0.003f;
        m_initial_mouse_position = mouse;

        if (Input::IsMouseButtonPressed(ALICE_MOUSE_BUTTON_MIDDLE))
            MousePan(delta);
        else if (Input::IsMouseButtonPressed(ALICE_MOUSE_BUTTON_LEFT))
            MouseRotate(delta);
        else if (Input::IsMouseButtonPressed(ALICE_MOUSE_BUTTON_RIGHT))
            MouseZoom(delta.y);
    }

    UpdateView();
}

void EditorCamera::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseScrolledEvent>(ALICE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
}

glm::vec3 EditorCamera::GetUpDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 EditorCamera::GetRightDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 EditorCamera::GetForwardDirection() const
{
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::quat EditorCamera::GetOrientation() const
{
    return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.0f));
}

void EditorCamera::SetViewportSize(float width, float height)
{ 
    m_viewport_width = width;
    m_viewport_height = height;
    UpdateProjection();
}

void EditorCamera::UpdateProjection()
{
    m_aspect_ratio = m_viewport_width / m_viewport_height;
    m_projection = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near_clip, m_far_clip);
}

void EditorCamera::UpdateView()
{
    m_position = CalculatePosition();

    glm::quat orientation = GetOrientation();
    m_view_matrix = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
    m_view_matrix = glm::inverse(m_view_matrix);
}

bool EditorCamera::OnMouseScroll(MouseScrolledEvent& event)
{
    float delta = event.GetYOffset();
    MouseZoom(delta);
    UpdateView();
    return false;
}

void EditorCamera::MousePan(const glm::vec2& delta)
{
    auto [x_speed, y_speed] = PanSpeed();
    m_focal_point += -GetRightDirection() * delta.x * x_speed * m_distance;
    m_focal_point += GetUpDirection() * delta.y * y_speed * m_distance;
}

void EditorCamera::MouseRotate(const glm::vec2& delta)
{
    float yaw_sign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    m_yaw += yaw_sign * delta.x * RotationSpeed();
    m_pitch += delta.y * RotationSpeed();
}

void EditorCamera::MouseZoom(float delta)
{
    m_distance -= delta * ZoomSpeed();
    if (m_distance < 1.0f)
    {
        m_focal_point += GetForwardDirection();
        m_distance = 1.0f;
    }
}

glm::vec3 EditorCamera::CalculatePosition() const
{
    return m_focal_point - GetForwardDirection() * m_distance;
}

std::pair<float, float> EditorCamera::PanSpeed() const
{
    float x = std::min(m_viewport_width / 1000.0f, 2.4f);
    float x_factor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_viewport_height / 1000.0f, 2.4f);
    float y_factor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { x_factor, y_factor };
}

float EditorCamera::RotationSpeed() const
{
    return 0.8f;
}

float EditorCamera::ZoomSpeed() const
{
    float distance = m_distance * 0.2f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min(speed, 100.0f);
    return speed;
}

} // namespace Alice
