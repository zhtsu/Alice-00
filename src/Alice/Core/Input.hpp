#pragma once

#include "Alice/PCH.hpp"

namespace Alice
{

class Input
{
public:
    //
    // 平台无关接口
    //
    inline static bool IsKeyPressed(int keycode)
    {
        return s_instance->IsKeyPressedImpl(keycode);
    }

    inline static bool IsMouseButtonPressed(int button)
    {
        return s_instance->IsMouseButtonPressedImpl(button);
    }

    inline static std::pair<float, float> GetMousePosition()
    {
        return s_instance->GetMousePositionImpl();
    }

    inline static float GetMouseX()
    {
        return s_instance->GetMouseXImpl();
    }

    inline static float GetMouseY()
    {
        return s_instance->GetMouseYImpl();
    }

protected:
    //
    // 由具体平台负责实现的接口
    //
    virtual bool IsKeyPressedImpl(int keycode) = 0;
    virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
    virtual std::pair<float, float> GetMousePositionImpl() = 0;
    virtual float GetMouseXImpl() = 0;
    virtual float GetMouseYImpl() = 0;

private:
    static Input* s_instance;
};

} // namespace Alice
