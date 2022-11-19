#pragma once

#include "Alice/Core/Input.hpp"

namespace Alice
{

//
// Windows 平台输入
//
class WindowsInput : public Input
{
protected:
    bool IsKeyPressedImpl(int keycode) override;
    bool IsMouseButtonPressedImpl(int button) override;
    std::pair<float, float> GetMousePositionImpl() override;
    float GetMouseXImpl() override;
    float GetMouseYImpl() override;
};

} // namespace Alice
