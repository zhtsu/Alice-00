#include "WindowsInput.hpp"

#include "Alice/Application/Application.hpp"
#include "GLFW/glfw3.h"

namespace Alice
{

Input* Input::s_instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
    auto window = static_cast<GLFWwindow*>(
        Application::Get().GetWindow().GetNativeWindow()
    );

    int state = glfwGetKey(window, keycode);

    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button)
{
    auto window = static_cast<GLFWwindow*>(
        Application::Get().GetWindow().GetNativeWindow()
    );

    auto state = glfwGetMouseButton(window, button);

    return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl()
{
    auto window = static_cast<GLFWwindow*>(
        Application::Get().GetWindow().GetNativeWindow()
    );

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

float WindowsInput::GetMouseXImpl()
{
    auto[x, y] = GetMousePositionImpl();

    return (float)x;
}

float WindowsInput::GetMouseYImpl()
{
    auto[x, y] = GetMousePositionImpl();

    return (float)y;
}

} // namespace Alice

