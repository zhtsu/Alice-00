#include "OrthographicCameraController.hpp"
#include "Alice/Core/Input.hpp"
#include "Alice/Core/KeyCodes.hpp"

namespace Alice
{

OrthographicCameraController::OrthographicCameraController(float aspect_ratio, bool rotation)
    : m_aspect_ratio(aspect_ratio),
    m_bounds({ -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level }),
    m_camera(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level),
    m_rotation(rotation)
{

}

void OrthographicCameraController::OnUpdate(Timestep ts)
{
    if (Input::IsKeyPressed(ALICE_KEY_A))
        m_camera_position.x -= m_camera_translation_speed * ts;
    else if (Input::IsKeyPressed(ALICE_KEY_D))
        m_camera_position.x += m_camera_translation_speed * ts;

    if (Input::IsKeyPressed(ALICE_KEY_W))
        m_camera_position.y += m_camera_translation_speed * ts;
    else if (Input::IsKeyPressed(ALICE_KEY_S))
        m_camera_position.y -= m_camera_translation_speed * ts;

    if (m_rotation)
    {
        if (Input::IsKeyPressed(ALICE_KEY_Q))
            m_camera_rotation += m_camera_rotation_speed * ts;
        else if (Input::IsKeyPressed(ALICE_KEY_E))
            m_camera_rotation -= m_camera_rotation_speed * ts;

        m_camera.SetRotation(m_camera_rotation);
    }

    m_camera.SetPosition(m_camera_position);
    m_camera_translation_speed = m_zoom_level;
}

void OrthographicCameraController::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<MouseScrolledEvent>(ALICE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(ALICE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(float width, float height)
{
    m_aspect_ratio = width / height;
    m_bounds = { -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level };
    m_camera.SetProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
}

void OrthographicCameraController::CalculateView()
{
    m_bounds = { -m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level };
    m_camera.SetProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
{
    m_zoom_level -= event.GetYOffset() * 0.5;
    m_zoom_level = std::max(m_zoom_level, 0.25f);
    
    CalculateView();
    
    return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
{
    m_aspect_ratio = (float)event.GetWidth() / (float)event.GetHeight();
    
    OnResize((float)event.GetWidth(), (float)event.GetHeight());

    return false;
}

} // namespace Alice
