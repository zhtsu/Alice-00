#include "EditorCamera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Alice
{

EditorCamera::EditorCamera(float fov, float aspect_ratio, float near_clip, float far_clip)
{

}

void EditorCamera::OnUpdate(Timestep ts)
{

}

void EditorCamera::OnEvent(Event& event)
{

}

glm::vec3 EditorCamera::GetUpDirection() const
{

}

glm::vec3 EditorCamera::GetRightDirection() const
{

}

glm::vec3 EditorCamera::GetForwardDirection() const
{

}

glm::quat EditorCamera::GetOrientation() const
{

}

void EditorCamera::UpdateProjection()
{

}

void EditorCamera::UpdateView()
{

}

bool EditorCamera::MouseScroll(MouseScrolledEvent& event)
{

}

void EditorCamera::MousePan(const glm::vec2& delta)
{

}

void EditorCamera::MouseRotate(const glm::vec2& delta)
{

}

void EditorCamera::MouseZoom(float delta)
{

}

glm::vec3 EditorCamera::CalculatePosition() const
{

}

std::pair<float, float> EditorCamera::PanSpeed() const
{

}

float EditorCamera::RotationSpeed() const
{

}

float EditorCamera::ZoomSpeed() const
{

}

} // namespace Alice
