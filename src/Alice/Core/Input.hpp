#pragma once

#include "Alice/PCH.hpp"

namespace Alice
{

class Input
{
public:
    static bool IsKeyPressed(int keycode);
    static bool IsMouseButtonPressed(int button);
    static std::pair<float, float> GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

} // namespace Alice
